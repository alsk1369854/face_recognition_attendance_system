#include "WebcamController.h"

#include <WiFi.h>
#include <FS.h>
#include "SPIFFS.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "ScreenController.h"

// Camera model: CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


// WiFi config json file
/*{
  "SSID": "AP_SSID",
  "PASS": "password"
}*/
#define WIFI_CONFIG_JSON_PATH "/WiFi_config.json"


// TFT Color
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */

// Static Public Variable
String WebcamController::wifiManagerAPName = "WebCam_AP";
String WebcamController::wifiManagerAPPassword = "password";
char WebcamController::autoConnectWifiSSID[50] = "WebCam_SSID";
char WebcamController::autoConnectWifiPASS[50] = "password";
int WebcamController::rebootCountdownTime = 5; // 5 Sec 
unsigned long WebcamController::wifiConnectTimeout = 5000; // 5 Sec
unsigned long WebcamController::displayDelayTime = 3000; // 3 Sec
bool WebcamController::shouldSaveWifiConfigFile = false;
String WebcamController::deviceID = "";
String WebcamController::postImageURL = "";


WiFiManager wm;


void saveWifiConfigCallback(){
    WebcamController::shouldSaveWifiConfigFile = true;
}

void wifiConfigModeCallback(WiFiManager *myWiFiManager){
    Serial.println("Start WiFiManager AP...");
    ScreenController::displayWiFiManagerAPMInfo(WebcamController::wifiManagerAPName, WebcamController::wifiManagerAPPassword, WiFi.softAPIP().toString());
}


void WebcamController::init(){
  ScreenController::init();
  
  // 初始化 File System
  initSPIFFS();

  // 初始化攝像機
  initCamera();
}


void WebcamController::initCamera(){
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.frame_size = FRAMESIZE_QVGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_vflip(s, 1);
}


void WebcamController::initSPIFFS(){
  if(SPIFFS.begin(true)){
    Serial.println("SPIFFS loading completed");
    bool wifiConfigFileExists = loadWifiConfigFile();
    if(!wifiConfigFileExists){
      saveWifiConfigFile();
    }
  }else{
    Serial.println("SPIFFS failed to load");
  }  
}


void WebcamController::wifiConnect(){
  unsigned long startingTime = millis();
  WiFi.begin(autoConnectWifiSSID, autoConnectWifiPASS);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    if((millis() - startingTime) > wifiConnectTimeout){
      initWifiManagerMode();
    }
  }
}


void WebcamController::saveWifiConfigFile(){
  Serial.println("Saving Wifi configuration...");
  
  // create a JSON document
  StaticJsonDocument<512> json;
  json["SSID"] = autoConnectWifiSSID;
  json["PASS"] = autoConnectWifiPASS;

  File wifiConfigFile = SPIFFS.open(WIFI_CONFIG_JSON_PATH, "w");
  if(wifiConfigFile){
    serializeJsonPretty(json, Serial);
    if(serializeJson(json, wifiConfigFile) == 0){
      // Error writing file
      Serial.println("Failed to write to file");
    }
  }else{
    // Failed to open WiFi config
    Serial.println("Failed to open WiFi config file for writing");
  }

  wifiConfigFile.close();
}


bool WebcamController::loadWifiConfigFile(){
  Serial.println("Loading Wifi configuration...");

  if(SPIFFS.exists(WIFI_CONFIG_JSON_PATH)){
    // Exists WiFi config json
    File wifiConfigFile = SPIFFS.open(WIFI_CONFIG_JSON_PATH, "r");
    if(wifiConfigFile){
      StaticJsonDocument<512> json;
      DeserializationError error = deserializeJson(json, wifiConfigFile);
      serializeJsonPretty(json, Serial);
      if(!error){
        // Parsing Json
        strcpy(autoConnectWifiSSID, json["SSID"]);
        strcpy(autoConnectWifiPASS, json["PASS"]);
        // autoConnectWifiPASS = json["PASS"].as<int>();

        Serial.println("Get WiFi Config Json");
        wifiConfigFile.close();
        return true;
      }else{
        // Error loading Json data
        Serial.println("Failed to load json config");
      }
    }else{
      // Failed to open WiFi config
      Serial.println("Failed to open WiFi config file for reading");
    }
    wifiConfigFile.close();
  }
  return false;
}


void WebcamController::initWifiManagerMode(){
  wm.setDebugOutput(false);
  
  wm.resetSettings();

  wm.setSaveConfigCallback(saveWifiConfigCallback);

  wm.setAPCallback(wifiConfigModeCallback);

  WiFiManagerParameter ssid_text_box("key_ssid", "Enter auto connect SSID", autoConnectWifiSSID, 50);
  WiFiManagerParameter password_text_box("key_pass", "Enter auto connect Password", autoConnectWifiPASS, 50);

  wm.addParameter(&ssid_text_box);
  wm.addParameter(&password_text_box);

//  if(wifiForceConfig){
    if(!wm.startConfigPortal((const char*)wifiManagerAPName.c_str(), (const char*)wifiManagerAPPassword.c_str())){
      Serial.println("Wifi Manager conntect failed");
      ScreenController::displayWifiConnectionFailedAndRebootAfterCountdown(rebootCountdownTime);
      ESP.restart();
      delay(5000);
    }
//  }else{
//    if(!wm.autoConnect((const char*)wifiManagerAPName.c_str(), (const char*)wifiManagerAPPassword.c_str())){
//      Serial.println("Wifi Manager conntect failed");
//      ScreenController::displayWifiConnectionFailedAndRebootAfterCountdown(rebootCountdownTime);
//      ESP.restart();
//      delay(5000);
//    }
//  }

  strncpy(autoConnectWifiSSID, ssid_text_box.getValue(), sizeof(autoConnectWifiSSID));
  strncpy(autoConnectWifiPASS, password_text_box.getValue(), sizeof(autoConnectWifiPASS));

  if(shouldSaveWifiConfigFile){
    saveWifiConfigFile();
  }
}


void WebcamController::parsingResult(String response){
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, response);
  JsonArray array = doc.as<JsonArray>();
  JsonObject detectionResult = doc.as<JsonObject>();
  JsonArray UnknownFaceValueList = detectionResult["unknown_face_value_list"];
  JsonArray KnownFaceValueList = detectionResult["known_face_value_list"];
  // Draw unknown face locations as yellow rectangles
  for(JsonVariant v : UnknownFaceValueList){
    JsonObject item = v.as<JsonObject>();
    int x1 = item["x1"];
    int y1 = item["y1"];
    int x2 = item["x2"];
    int y2 = item["y2"];
    ScreenController::drawFaceRect(x1, y1, x2 - x1, y2 - y1, 3, TFT_YELLOW);
  }
  
  // Draw known face locations as green rectangles
  for(JsonVariant v : KnownFaceValueList){
    JsonObject item = v.as<JsonObject>();
    int x1 = item["x1"];
    int y1 = item["y1"];
    int x2 = item["x2"];
    int y2 = item["y2"];
    const char* personName = item["name"];
    ScreenController::drawFaceRect(x1, y1, x2 - x1, y2 - y1, 3, TFT_GREEN);
    ScreenController::drawString(personName, x1+4, y2, 4);
  }

  if(KnownFaceValueList.size() > 0){
    delay(displayDelayTime);
  }
}

void WebcamController::postingImage(camera_fb_t *fb){
  HTTPClient client;
  
  client.begin(postImageURL);
  client.addHeader("Content-Type", "image/jpeg");
  int httpResponseCode = client.POST(fb->buf, fb->len);
  if(httpResponseCode == 200){
    String response = client.getString();
    parsingResult(response);
  }else{
    //Error
    ScreenController::drawErrorString("Check Your Server", 8, 4);
    delay(displayDelayTime);
  }
  client.end();
}


void WebcamController::sendingImage(){
  camera_fb_t *fb = esp_camera_fb_get();
  if(!fb || fb->format != PIXFORMAT_JPEG){
    Serial.println("Camera Capture Failed!");
  }else{
    Serial.println("Camera Image Sending..."); 
    ScreenController::drawJpg(fb);
    
    postingImage(fb);
  }
  esp_camera_fb_return(fb);
}

void WebcamController::setDeviceID(String deviceID){
  WebcamController::deviceID = deviceID;
  WebcamController::wifiManagerAPName = String("Webcam_") + deviceID;
}

void WebcamController::setPostImageURL(String postImageURL){
  WebcamController::postImageURL = postImageURL;
}
