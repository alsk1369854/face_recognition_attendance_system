#include "WebcamController.h"


// Webcam info
String Webcam_Device_ID = "A001";
String Post_Image_Upload_URL = "http://192.168.1.37:8888/imageUpload/" + Webcam_Device_ID;


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
