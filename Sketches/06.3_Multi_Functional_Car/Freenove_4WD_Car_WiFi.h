#ifndef _FREENOVE_4WD_CAR_WIFI_H
#define _FREENOVE_4WD_CAR_WIFI_H
#include "esp_camera.h"

///////////////////WiFi command area////////////////////////////////////////
#define ENTER               '\n'                  //ENTER
#define INTERVAL_CHAR       '#'                   //INTERVAL CHAR
#define CMD_MOTOR           "CMD_MOTOR"           //Motor control command
#define CMD_POWER           "CMD_POWER"           //Power control command
#define CMD_BUZZER          "CMD_BUZZER"          //BUZZER control command
#define CMD_SERVO           "CMD_SERVO"           //SERVO control command
#define CMD_CAMERA          "CMD_CAMERA"          //SERVO control command

#define CMD_LED_MOD         "CMD_LED_MOD"         //WS2812 control command
#define CMD_LED             "CMD_LED"             //WS2812 color command
#define CMD_MATRIX_MOD      "CMD_MATRIX_MOD"      //LED MATRIX control command
#define CMD_VIDEO           "CMD_VIDEO"           //VIDEO control command
#define CMD_LIGHT           "CMD_LIGHT"           //LIGHT Car control command
#define CMD_TRACK           "CMD_TRACK"           //TRACK Car control command
#define CMD_CAR_MODE        "CMD_CAR_MODE"        //Car control command

extern char* ssid_Router             ;    //Modify according to your router name
extern char* password_Router         ;    //Modify according to your router password
extern char* ssid_AP                 ;    //ESP32 turns on an AP and calls it Sunshine
extern char* password_AP             ;    //Set your AP password for ESP32 to Sunshine
void WiFi_Setup(bool WiFi_Mode)      ;    //Initialize WiFi function
void loopTask_WTD(void *pvParameters);

///////////////////Camera drive area////////////////////////////////////////
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

extern framesize_t frame_size;
bool cameraSetup(void);                    //Camera initialization
void camera_vflip(bool enable);            //Set the camera to flip up and down
void camera_hmirror(bool enable);          //Set the camera to flip left and right




#endif




//
