/**********************************************************************
  Filename    : Tracking_Car.ino
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2024/08/12
**********************************************************************/

#include <Arduino.h>
#include "Freenove_4WD_Car_For_ESP32.h"

#define SPEED_LV4   ( 4000 )
#define SPEED_LV3   ( 3000 )
#define SPEED_LV2   ( 2500 )
#define SPEED_LV1   ( 1500 )

void setup()
{
  Track_Setup();   //Trace module initialization
  PCA9685_Setup(); //Motor drive initialization
  Emotion_Setup();
}

void loop()
{
  Track_Read();
  switch (sensorValue[3])
  {
    case 2:   //010
    case 5:   //101
      showArrow(1, 100);
      Motor_Move(SPEED_LV1, SPEED_LV1, SPEED_LV1, SPEED_LV1);    //Move Forward
      break;
    case 0:   //000
    case 7:   //111
      eyesBlink1(100);
      Motor_Move(0, 0, 0, 0);                                    //Stop
      break;
    case 4:   //100
    case 6:   //110
      wheel(2, 100);
      Motor_Move(SPEED_LV4, SPEED_LV4 , - SPEED_LV3, -SPEED_LV3);//Turn Right
      break;
    case 1:   //001
    case 3:   //011
      wheel(1, 100);
      Motor_Move(-SPEED_LV3, -SPEED_LV3, SPEED_LV4, SPEED_LV4);  //Turn Left
      break;
    default:
      break;
  }
}
