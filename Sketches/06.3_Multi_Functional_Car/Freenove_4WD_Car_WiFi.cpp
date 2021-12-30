#include "Freenove_4WD_Car_WiFi.h"
#include "Freenove_4WD_Car_For_ESP32.h"

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

///////////////////WiFi drive area////////////////////////////////////////
char* ssid_Router     =   "********";    //Modify according to your router name
char* password_Router =   "********";    //Modify according to your router password
char* ssid_AP         =   "Sunshine";    //ESP32 turns on an AP and calls it Sunshine
char* password_AP     =   "Sunshine";    //Set your AP password for ESP32 to Sunshine
bool WiFi_MODE = 1;

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

//Initialize WiFi function
void WiFi_Setup(bool WiFi_Mode)
{
  if (WiFi_Mode == 0)
  {
    WiFi_MODE = 0;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_Router, password_Router);
    WiFi.setSleep(false);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);

    Serial.print("\nWaiting for WiFi... ");
    while (WiFi.isConnected() != true) {
      Serial.print(".");
      delay(500);
    }
    IPAddress local_ip = WiFi.localIP();
    Serial.println("");
    Serial.println("\nWiFi connected");
    Serial.print("Use your phone to connect to WiFi: ");
    Serial.println(ssid_Router);
    Serial.print("\nThe password for WiFi is: ");
    Serial.println(password_Router);
    Serial.print("\nThen you can enter: '");
    Serial.print(local_ip);
    Serial.println("' to connect the car in Freenove app.");
    Buzzer_Alarm(1);
    delay(100);
    Buzzer_Alarm(0);
  }
  else
  {
    WiFi_MODE = 1;
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid_AP, password_AP);

    Serial.print("\nUse your phone to connect to WiFi: ");
    Serial.println(ssid_AP);
    Serial.print("\nThe password for WiFi is: ");
    Serial.println(password_AP);
    Serial.print("\nThen you can enter: '");
    Serial.print(local_IP);
    Serial.println("' to connect the car in Freenove app.");
    Buzzer_Alarm(1);
    delay(100);
    Buzzer_Alarm(0);
  }
}

int wtdFlag = 0;
void loopTask_WTD(void *pvParameters) {
  while (1)
  {
    if (WiFi_MODE == 0)
    {
      if ((WiFi.isConnected() == 0) && wtdFlag == 0)
      {
        delay(100);
      }
      else if ((WiFi.isConnected() != 0) && wtdFlag == 0)
      {
        wtdFlag = 1;
        delay(100);
      }
      else if ((WiFi.isConnected() == 0) && wtdFlag == 1)
      {
        wtdFlag = 0;
        ESP.restart();
      }
    }
    else
    {
      if ((WiFi.softAPgetStationNum() == 0) && wtdFlag == 0)
      {
        delay(100);
      }
      else if ((WiFi.softAPgetStationNum() != 0) && wtdFlag == 0)
      {
        wtdFlag = 1;
        delay(100);
      }
      else if ((WiFi.softAPgetStationNum() == 0) && wtdFlag == 1)
      {
        wtdFlag = 0;
        ESP.restart();
      }
    }
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
