/**********************************************************************
  Product     : Freenove 4WD Car for ESP32
  Description : use servo.
  Auther      : www.freenove.com
  Modification: 2020/12/18
**********************************************************************/
#include "Freenove_4WD_Car_For_ESP32.h"

void setup()
{
  Servo_Setup();    //Servo initialization
}

void loop()
{
  Servo_1_Angle(90);//Set the Angle value of servo 1 to 90°
  Servo_2_Angle(90);//Set the Angle value of servo 2 to 90°
  delay(1000);
}
