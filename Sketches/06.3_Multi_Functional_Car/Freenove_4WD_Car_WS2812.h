#ifndef _FREENOVE_4WD_CAR_WS2812_H
#define _FREENOVE_4WD_CAR_WS2812_H

#ifndef _FREENOVE_WS2812_LIB_FOR_ESP32_H
#define _FREENOVE_WS2812_LIB_FOR_ESP32_H
#include <Freenove_WS2812_Lib_for_ESP32.h>
#endif

#define WS2812_PIN 32        //Control pins for ESP32
extern int ws2812_task_mode; //WS2812 run mode                

void WS2812_Setup(void);                                                                                   //WS2812 initialization function
void WS2812_Show(int mode);                                                                                //WS2812 non-blocking display function
void WS2812_Set_Color_1(int number, unsigned char color_1,unsigned char color_2,unsigned char color_3);    //Set the display color1 for WS2812
void WS2812_Set_Color_2(int number, unsigned char color_1,unsigned char color_2,unsigned char color_3);    //Set the display color2 for WS2812
void WS2812_SetMode(int mode);                                                                             //WS2812 set mode


#endif













//
