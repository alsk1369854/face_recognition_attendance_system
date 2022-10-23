#ifndef WebcamController_h

#define WebcamController_h

#include <Arduino.h>
#include "esp_camera.h"
#include <WiFiManager.h>


class WebcamController{
  
  public:
//    static ScreenController screenController;
    static String wifiManagerAPName;
    static String wifiManagerAPPassword;
    static char autoConnectWifiSSID[50];
    static char autoConnectWifiPASS[50];
    static int rebootCountdownTime;
    static unsigned long wifiConnectTimeout;
    static unsigned long displayDelayTime;
    static bool shouldSaveWifiConfigFile;
    static String deviceID;
    static String postImageURL;
    
//    WebcamController(String, String);

//    static void saveWifiConfigCallback();
//    static void wifiConfigModeCallback(WiFiManager*);
    static void init();
    static void initCamera();
    static void initSPIFFS();
    static void wifiConnect();
    static void saveWifiConfigFile();
    static bool loadWifiConfigFile();
    static void initWifiManagerMode();
    static void parsingResult(String);
    static void postingImage(camera_fb_t*);
    static void sendingImage();
    static void setDeviceID(String);
    static void setPostImageURL(String);

      
};

#endif
