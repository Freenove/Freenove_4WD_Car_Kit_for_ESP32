/**********************************************************************
  Product     : Freenove 4WD Car for ESP32
  Description : Use led matrix.
  Auther      : www.freenove.com
  Modification: 2020/12/18
**********************************************************************/

#include "Freenove_VK16K33_Lib_For_ESP32.h"

#define EMOTION_ADDRESS 0x71
#define EMOTION_SDA     13
#define EMOTION_SCL     14

Freenove_ESP32_VK16K33 matrix = Freenove_ESP32_VK16K33();

byte x_array[][8] = {//Put the data into the left LED matrix
  //////////////////////////////////////////////
  0x00, 0x18, 0x24, 0x42, 0x42, 0x24, 0x18, 0x00, 
  0x00, 0x18, 0x24, 0x5A, 0x5A, 0x24, 0x18, 0x00,
  //////////////////////////////////////////////
};

byte y_array[][8] = {//Put the data into the right LED matrix
  //////////////////////////////////////////////
  0x00, 0x18, 0x24, 0x42, 0x42, 0x24, 0x18, 0x00,
  0x00, 0x18, 0x24, 0x5A, 0x5A, 0x24, 0x18, 0x00,
  /////////////////////////////////////////////////
};

void setup()
{
  matrix.init(EMOTION_ADDRESS, EMOTION_SDA, EMOTION_SCL);
  matrix.setBlink(VK16K33_BLINK_OFF);
}

void loop()
{
  int count = sizeof(x_array) / sizeof(x_array[0]);
  for (int i = 0; i < count; i++)
  {
    matrix.showStaticArray(x_array[i], y_array[i]);
    delay(500);
  }
}
