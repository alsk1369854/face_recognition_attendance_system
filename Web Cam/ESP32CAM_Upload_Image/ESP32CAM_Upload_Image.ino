#include "esp_camera.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define GFXFF 1
#define FSB9 &FreeSerifBold9pt7b

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

TFT_eSPI tft = TFT_eSPI();

const char* ssid = "chts320";
const char* password = "12345678";
const unsigned long timeout = 10000; // 30 seconds
bool isWiFiConnected = false;

void cameraInit(){
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
}

bool wifiConnect(){
  unsigned long startingTime = millis();
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    if((millis() - startingTime) > timeout){
      return false;  
    }
  }
  return true;
}

void parsingResult(String response){
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, response);
  JsonArray array = doc.as<JsonArray>();
  int yPos = 4;
  /*
  for(JsonVariant v : array){
    JsonObject object = v.as<JsonObject>();
    const char* description = object["description"];
    float score = object["score"];
    String label = "";
    label += description;
    label += ":";
    label += "score";

    tft.drawString(label, 8, yPos, GFXFF);
    yPos += 16;
  }
  */
  String label = response;
  tft.drawString(label, 8, yPos, GFXFF);
}

void postingImage(camera_fb_t *fb){
  HTTPClient client;
  client.begin("http://192.168.1.37:8888/imageUpdate");
  client.addHeader("Content-Type", "image/jpeg");
  int httpResponseCode = client.POST(fb->buf, fb->len);
  if(httpResponseCode == 200){
    String response = client.getString();
    parsingResult(response);
  }else{
    //Error
    tft.drawString("Check Your Server", 8, 4, GFXFF);
    delay(1000);
  }

  client.end();
  // WiFi.disconnect();
}

void showingImage(){
  camera_fb_t* fb = esp_camera_fb_get();
  if(!fb || fb->format != PIXFORMAT_JPEG){
    Serial.println("Camera Capture Failed!");
  }else{
    Serial.println("Camera Image to Display Here!"); 
    TJpgDec.drawJpg(0, 0, (const uint8_t*)fb->buf, fb->len);
  }
  esp_camera_fb_return(fb);
}

void sendingImage(){
  camera_fb_t* fb = esp_camera_fb_get();
  if(!fb || fb->format != PIXFORMAT_JPEG){
    Serial.println("Camera Capture Failed!");
  }else{
    Serial.println("Camera Image to Display Here!"); 
    TJpgDec.drawJpg(0, 0, (const uint8_t*)fb->buf, fb->len);

    if(isWiFiConnected){
      postingImage(fb);
    }else{
      tft.drawString("Check Wifi credential!", 8, 4, GFXFF);
      delay(1000);
    }
  }
  esp_camera_fb_return(fb);
}

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
  if(y >= tft.height()) return 0;
  tft.pushImage(x, y, w, h, bitmap);
  return 1;  
}

void displayInit(){
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);
  tft.setFreeFont(FSB9);

  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);
}


void setup() {
  Serial.begin(115200);
  displayInit();
  cameraInit();
  isWiFiConnected = wifiConnect();
}

void loop() {
  sendingImage();
}
