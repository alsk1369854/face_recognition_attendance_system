#include "WebcamController.h"

/*  PIN Wiring
|   ILI9341   |   ESP32-CAM   |
------------------------------
|   TFT_MISO  |       12      |
|   TFT_MOSI  |       13      |
|   TFT_SCLK  |       14      |
|   TFT_CS    |       2       |
|   TFT_DC    |       15      |
|   TFT_RST   |       16      |
*/

// Webcam info
String Webcam_Device_ID = "A001";
String Post_Image_Upload_URL = "http://192.168.1.37:8888/face_recognition/" + Webcam_Device_ID;


void setup() {
  Serial.begin(115200);
  WebcamController::init();
  WebcamController::setDeviceID(Webcam_Device_ID);
  WebcamController::setPostImageURL(Post_Image_Upload_URL);
  WebcamController::wifiConnect();
}

void loop() {
  // WiFi Config Reset
  // webcamController::initWifiManagerMode();

  WebcamController::sendingImage();
}
