/**********************************************************************
  Filename    : Photosensitive_Car.ino
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2024/08/12
**********************************************************************/
#include <Arduino.h>
#include "Freenove_4WD_Car_For_ESP32.h"

int photosensitive_sensitivity = 100;//Set the sensitivity of the photosensitive resistor trolley
int photosensitive_init_value = 0;   //Set the car's initial environment ADC value

void setup()
{
  Emotion_Setup();
  Buzzer_Setup();
  Photosensitive_Setup();  //Photosensitive initialization
  PCA9685_Setup();           //Initialize PCA9685 to control motor
  photosensitive_init_value = Get_Photosensitive();
  //Buzzer_Alert(1,1);       //The buzzer beeps to prompt the user to start playing
}

void loop()
{
  //There is a light source on the left side of the car
  if (Get_Photosensitive() < (photosensitive_init_value - photosensitive_sensitivity))
  {
    wheel(2,100);
    Motor_Move(-2000, -2000, 2000, 2000);
  }
  //There is a light source on the right side of the car
  else if (Get_Photosensitive() > (photosensitive_init_value + photosensitive_sensitivity))
  {
    wheel(1,100);
    Motor_Move(2000, 2000, -2000, -2000);
  }
  //The light is in the middle of the car
  else
  {
    eyesBlink(100);
    Motor_Move(0, 0, 0, 0);
  } 
}
