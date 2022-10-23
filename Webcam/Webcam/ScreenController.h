#ifndef ScreenController_h

#define ScreenController_h

#include <Arduino.h>
#include "esp_camera.h"


class ScreenController {
  public:
    static void init();
//    static bool tft_output(int16_t, int16_t, uint16_t, uint16_t, uint16_t*);
    static void drawString(const char*, uint16_t, uint16_t, int);
    static void drawErrorString(const char*, uint16_t, uint16_t);
    static void drawFaceRect(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint32_t);
    static void drawJpg(camera_fb_t*);
    static void displayWifiConnectionFailedAndRebootAfterCountdown(int);
    static void displayWiFiManagerAPMInfo(String, String, String);
    
};

#endif
