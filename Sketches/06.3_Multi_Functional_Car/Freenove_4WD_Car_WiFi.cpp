#include "Freenove_4WD_Car_WiFi.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

///////////////////WiFi drive area////////////////////////////////////////
char* ssid_Router     =   "********";    //Modify according to your router name
char* password_Router =   "********";    //Modify according to your router password
char* ssid_AP         =   "Sunshine";    //ESP32 turns on an AP and calls it Sunshine
char* password_AP     =   "Sunshine";    //Set your AP password for ESP32 to Sunshine
//Initialize WiFi function
void WiFi_Setup(void)
{
  WiFi.softAP(ssid_AP, password_AP);//Turn on the router
  WiFi.disconnect(true);
  WiFi.begin(ssid_Router, password_Router);
  Serial.print("Connecting ");
  Serial.print(ssid_Router);
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    timeout++;
    WiFi.begin(ssid_Router, password_Router);
    if (timeout >= 20)
      break;
  }
  IPAddress ap_ip = WiFi.softAPIP();
  IPAddress local_ip = WiFi.localIP();
  if (timeout >= 20)
  {
    Serial.println("\nWiFi connect failed");
    Serial.print("Use your phone to connect to WiFi: '");
    Serial.print(ssid_AP);
    Serial.print("'\nThe password for WiFi is: '");
    Serial.print(password_AP);
    Serial.print("'\nThen you can enter: '");
    Serial.print(ap_ip);
    Serial.println("' to connect the car in Freenove app.");
  }
  else
  {
    Serial.println("\nWiFi connected.");
    Serial.print("Use your phone to connect to WiFi: '");
    Serial.print(ssid_Router);
    Serial.print("'\nThe password for WiFi is: '");
    Serial.print(password_Router);
    Serial.print("'\nThen you can enter: '");
    Serial.print(local_ip);
    Serial.println("' to connect the car in Freenove app.");
  }
}

///////////////////Camera drive area///////////////////////////////////
framesize_t frame_size   =   FRAMESIZE_CIF;      //The default is to use the image size of FRAMESIZE CIF
//Camera initialization
bool cameraSetup(void)
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_1;//From modification
  config.ledc_timer = LEDC_TIMER_1;//From modification
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
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = frame_size;
  config.jpeg_quality = 10;
  config.fb_count = 1;
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return 0;
  }
  Serial.println("Camera configuration complete!");
  return 1;
}
//Set the camera to flip up and down
void camera_vflip(bool enable)
{
  sensor_t * s = esp_camera_sensor_get();
  s->set_vflip(s, enable);
}
//Set the camera to flip left and right
void camera_hmirror(bool enable)
{
  sensor_t * s = esp_camera_sensor_get();
  s->set_hmirror(s, enable);
}















//
