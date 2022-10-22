#include <FS.h>
#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiManager.h>
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi config json file
/*{
  "SSID": "WebCam_SSID",
  "PASS": "password"
}*/
#define WIFI_CONFIG_JSON_PATH "/WiFi_config.json"

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

// TFT_eSPI Style
#define GFXFF              1      // font-style
#define FSB9 &FreeSerifBold9pt7b // font-family


// WebCam info
String WEBCAM_DEVICE_ID = "A001";
String SERVER_IMAGE_UPLOAD_URL = "http://192.168.1.37:8888/imageUpload/";

// Const Value
String serverImageUploadUrl = SERVER_IMAGE_UPLOAD_URL + WEBCAM_DEVICE_ID;
const unsigned long wifiConnectTimeout = 5000; // 5 Sec
const unsigned long displayDelayTime = 3000; // 3 Sec

// Default Wifi Connection ID and Password
char autoConnectWifiSSID[50] = "WebCam_SSID";
char autoConnectWifiPASS[50] = "password";
String wifiManagerAPName = String("WebCam_") + WEBCAM_DEVICE_ID;
String wifiManagerAPPassword = "password";
bool shouldSaveWifiConfigFile = false;
bool wifiForceConfig = false;
int rebootCountdownTime = 5; // 5 Sec 

WiFiManager wm;
TFT_eSPI tft = TFT_eSPI();

// Start Class ScreenController ==========================================
class ScreenController {
  public:
    void init();
    static bool tft_output(int16_t, int16_t, uint16_t, uint16_t, uint16_t*);
    void drawString(const char*, uint16_t, uint16_t, int);
    void drawFaceRect(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint32_t);
    void drawJpg(camera_fb_t*);
    void displayWifiConnectionFailedAndRebootAfterCountdown(int);
    void displayWiFiManagerAPMInfo(String, String, String);
  
};

bool ScreenController::tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
  if(y >= tft.height()) return 0;
  tft.pushImage(x, y, w, h, bitmap);
  return 1;  
}

void ScreenController::init(){
  tft.begin();
  
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(FSB9);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Loading...", 70, 160, 4);
  
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(this->tft_output);
}

void ScreenController::drawString(const char* text, uint16_t x, uint16_t y, int fontStyle){
  tft.drawString(text, x, y, fontStyle);
}

void ScreenController::drawFaceRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t pixel, uint32_t color){
  for (int i = 0; i < pixel; i++) {
       tft.drawRect(x + i, y + i, w - i*2, h - i*2, color);
  }
}

void ScreenController::drawJpg(camera_fb_t *fb){
  tft.setRotation(1);
  TJpgDec.drawJpg(0, 0, (const uint8_t*)fb->buf, fb->len);
  tft.setRotation(0);
}

void ScreenController::displayWifiConnectionFailedAndRebootAfterCountdown(int countdown){
  Serial.println("In Countdown");
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Connection Failed", 15, 110, 4);
  tft.drawString("Reboot After", 50, 140, 4);
  
  for(int number = countdown; number > 0; number--){  
    tft.setTextColor(TFT_WHITE);
    tft.drawString(String(number), 105, 200, 7);
    delay(1000);
    if(number > 1){
      tft.setTextColor(TFT_BLACK);
      tft.drawString(String(number), 105, 200, 7);
    }
  } 
}

void ScreenController::displayWiFiManagerAPMInfo(String apName, String apPassword, String apIP){
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Please set WiFi", 35, 120, 4);
  String labelAPName = "AP_ID: " + apName;
  tft.drawString(labelAPName, 25, 170, GFXFF);
  String labelAPPassword = "PASS:  " + apPassword;
  tft.drawString(labelAPPassword, 25, 200, GFXFF);
  String labelIP_Addr = String("Access: ") + apIP;
  tft.drawString(labelIP_Addr, 25, 230, GFXFF);
}

ScreenController screenController;
// End Class ScreenController ==========================================



// Start WiFi auto Connect =================================== 
void initSPIFFS(){
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

void saveWifiConfigFile(){
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

bool loadWifiConfigFile(){
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

void saveWifiConfigCallback(){
    shouldSaveWifiConfigFile = true;
}

void wifiConfigModeCallback(WiFiManager *myWiFiManager){
  screenController.displayWiFiManagerAPMInfo(wifiManagerAPName, wifiManagerAPPassword, WiFi.softAPIP().toString());
}

void initWifiManagerMode(){
  wm.setDebugOutput(false);
  
  wm.resetSettings();

  wm.setSaveConfigCallback(saveWifiConfigCallback);

  wm.setAPCallback(wifiConfigModeCallback);

  WiFiManagerParameter ssid_text_box("key_ssid", "Enter auto connect SSID", autoConnectWifiSSID, 50);
  WiFiManagerParameter password_text_box("key_pass", "Enter auto connect Password", autoConnectWifiPASS, 50);

  wm.addParameter(&ssid_text_box);
  wm.addParameter(&password_text_box);

  if(wifiForceConfig){
    if(!wm.startConfigPortal((const char*)wifiManagerAPName.c_str(), (const char*)wifiManagerAPPassword.c_str())){
      Serial.println("Wifi Manager conntect failed");
      screenController.displayWifiConnectionFailedAndRebootAfterCountdown(rebootCountdownTime);
      ESP.restart();
      delay(5000);
    }
  }else{
    if(!wm.autoConnect((const char*)wifiManagerAPName.c_str(), (const char*)wifiManagerAPPassword.c_str())){
      Serial.println("Wifi Manager conntect failed");
      screenController.displayWifiConnectionFailedAndRebootAfterCountdown(rebootCountdownTime);
      ESP.restart();
      delay(5000);
    }
  }

  strncpy(autoConnectWifiSSID, ssid_text_box.getValue(), sizeof(autoConnectWifiSSID));
  strncpy(autoConnectWifiPASS, password_text_box.getValue(), sizeof(autoConnectWifiPASS));

  if(shouldSaveWifiConfigFile){
    saveWifiConfigFile();
  }
}
// End WiFi auto Connect =================================== 


void camera_init(){
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


void wifiConnect(){
  unsigned long startingTime = millis();
  WiFi.begin(autoConnectWifiSSID, autoConnectWifiPASS);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    if((millis() - startingTime) > wifiConnectTimeout){
      initWifiManagerMode();
    }
  }
}


void parsingResult(String response){
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
    screenController.drawFaceRect(x1, y1, x2 - x1, y2 - y1, 3, TFT_YELLOW);
  }
  
  // Draw known face locations as green rectangles
  for(JsonVariant v : KnownFaceValueList){
    JsonObject item = v.as<JsonObject>();
    int x1 = item["x1"];
    int y1 = item["y1"];
    int x2 = item["x2"];
    int y2 = item["y2"];
    const char* personName = item["name"];
    screenController.drawFaceRect(x1, y1, x2 - x1, y2 - y1, 3, TFT_GREEN);
    screenController.drawString(personName, x1+4, y2, 4);
  }

  if(KnownFaceValueList.size() > 0){
    delay(displayDelayTime);
  }
}

void postingImage(camera_fb_t *fb){
  HTTPClient client;
  client.begin(serverImageUploadUrl);
  client.addHeader("Content-Type", "image/jpeg");
  int httpResponseCode = client.POST(fb->buf, fb->len);
  if(httpResponseCode == 200){
    String response = client.getString();
    parsingResult(response);
  }else{
    //Error
    screenController.drawString("Check Your Server", 8, 4, GFXFF);
    delay(displayDelayTime);
  }
  client.end();
}


void sendingImage(){
  camera_fb_t *fb = esp_camera_fb_get();
  if(!fb || fb->format != PIXFORMAT_JPEG){
    Serial.println("Camera Capture Failed!");
  }else{
    Serial.println("Camera Image to Display Here!"); 
    screenController.drawJpg(fb);
    
    postingImage(fb);
  }
  esp_camera_fb_return(fb);
}


void setup() {
  Serial.begin(115200);
  screenController.init();
  initSPIFFS();
  wifiConnect();
  camera_init();
}

void loop() {
  // WiFi Config Reset
  // wifiForceConfig = true;
  // initWifiManagerMode();
  
  sendingImage();
}
