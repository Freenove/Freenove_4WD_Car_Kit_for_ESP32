/**********************************************************************
  Product     : Freenove 4WD Car for ESP32
  Description : use servo.
  Auther      : www.freenove.com
  Modification: 2020/12/18
**********************************************************************/
#include "Freenove_4WD_Car_For_ESP32.h"

void setup()
{
  PCA9685_Setup();       //Initializes the chip that controls the motor
  Servo_1_Angle(90);//Set servo 1 Angle
  Servo_2_Angle(90);//Set servo 2 Angle
  delay(1000);
}

void loop()
{
  // Servo 1 motion path; 90°- 0°- 180°- 90°
  Servo_Sweep(1, 90, 0);
  Servo_Sweep(1, 0, 180);
  Servo_Sweep(1, 180, 90);

  // Servo 2 motion path; 90°- 150°- 90°
  Servo_Sweep(2, 90, 150);
  Servo_Sweep(2, 150, 90);
}
