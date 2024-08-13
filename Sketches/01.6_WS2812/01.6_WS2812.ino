/**********************************************************************
  Product     : Freenove 4WD Car for ESP32
  Description : Use WS2812.
  Auther      : www.freenove.com
  Modification: 2024/08/12
**********************************************************************/

#include "Freenove_WS2812_Lib_for_ESP32.h"

#define LEDS_COUNT  12    //Define the count of WS2812
#define LEDS_PIN  	32    //Define the pin number for ESP32
#define CHANNEL	    0     //Define the channels that control WS2812

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

void setup() {
  strip.begin();		  //Initialize WS2812
  strip.setBrightness(10);//Set the brightness of WS2812
}

void loop() {
  for (int j = 0; j < 255; j += 2) {
    for (int i = 0; i < LEDS_COUNT; i++) {
		strip.setLedColorData(i, strip.Wheel((i * 256 / LEDS_COUNT + j) & 255));//Set the color of the WS2812
    }
    strip.show();	      //Call WS2812 to display colors
    delay(5);
  }  
}
