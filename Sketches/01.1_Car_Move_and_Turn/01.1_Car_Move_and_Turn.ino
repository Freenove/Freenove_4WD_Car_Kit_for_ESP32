/**********************************************************************
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2020/12/18
**********************************************************************/
#include "Freenove_4WD_Car_For_ESP32.h"

void setup()
{
  PCA9685_Setup();       //Initializes the chip that controls the motor
}

void loop()
{
  Motor_Move(2000, 2000, 2000, 2000);     //go forward
  delay(1000);
  Motor_Move(0, 0, 0, 0);                 //stop
  delay(1000);
  Motor_Move(-2000, -2000, -2000, -2000); //go back
  delay(1000);
  Motor_Move(0, 0, 0, 0);                 //stop
  delay(1000);

  Motor_Move(-2000, -2000, 2000, 2000);   //turn left
  delay(1000);
  Motor_Move(0, 0, 0, 0);                 //stop
  delay(1000);
  Motor_Move(2000, 2000, -2000, -2000);   //turn right
  delay(1000);
  Motor_Move(0, 0, 0, 0);                 //stop
  delay(1000);
}
