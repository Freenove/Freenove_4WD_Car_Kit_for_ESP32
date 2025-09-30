#ifndef _CAMERA_STREAM_H
#define _CAMERA_STREAM_H

#include <Arduino.h>
#include "esp_camera.h"

#define MODE_AP  1
#define MODE_STA 2

#define CMD_VIDEO           "CMD_VIDEO"
#define INTERVAL_CHAR       '#'
#define ENTER               '\n'

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

void WiFi_Setup_AP();
void WiFi_Setup_STA();

bool cameraSetup(void);
void loopTask_Camera(void *pvParameters);

#endif