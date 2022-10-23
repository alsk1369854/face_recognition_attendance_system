#include "ScreenController.h"

#include <SPI.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

// TFT_eSPI Style
#define GFXFF              1      // font-style
#define FSB9 &FreeSerifBold9pt7b // font-family

TFT_eSPI tft = TFT_eSPI();

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
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
  TJpgDec.setCallback(tft_output);
}


void ScreenController::drawString(const char* text, uint16_t x, uint16_t y, int fontStyle){
  tft.setTextColor(TFT_WHITE);
  tft.drawString(text, x, y, fontStyle);
}


void ScreenController::drawErrorString(const char* text, uint16_t x, uint16_t y){
  tft.setTextColor(TFT_RED);
  tft.drawString(text, x, y, GFXFF);
}


void ScreenController::drawFaceRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t pixel, uint32_t color){
  for (int i = 0; i < pixel; i++) {
       tft.drawRect(x + i, y + i, w - i*2, h - i*2, color);
  }
}


void ScreenController::drawJpg(camera_fb_t *fb){
  //  w320*h240 to w240*h320
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
  String labelAPPassword = "PASS  : " + apPassword;
  tft.drawString(labelAPPassword, 25, 200, GFXFF);
  String labelIP_Addr = String("Access: ") + apIP;
  tft.drawString(labelIP_Addr, 25, 230, GFXFF);
}
