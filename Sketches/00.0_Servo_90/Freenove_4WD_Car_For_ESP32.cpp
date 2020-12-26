#include <Arduino.h>
#include "Freenove_4WD_Car_For_ESP32.h"
#include <Wire.h>
/////////////////////Servo drive area///////////////////////////////////
int servo_1_offset=0; //Define the offset variable for servo 1
int servo_2_offset=0; //Define the offset variable for servo 2
PCA9685 PCA9685_SERVO;//Instantiate a PCA9685 object

//servo initialization
void Servo_Setup(void)
{
  Wire.begin(PCA9685_SERVO_SDA, PCA9685_SERVO_SCL);  
  PCA9685_SERVO.setupSingleDevice(Wire,PCA9685_ADDRESS);
  PCA9685_SERVO.setToFrequency(SERVO_FREQUENCY); 
}

//Set the rotation parameters of servo 1, and the parameters are 0-180 degrees
void Servo_1_Angle(float angle)
{
  angle=constrain(angle,0,180);
  angle=map(angle,0,180,500,2500);
  PCA9685_SERVO.setChannelServoPulseDuration(PCA9685_CHANNEL_0,int(angle)+servo_1_offset);
}

//Set the rotation parameters of servo 2, and the parameters are 0-180 degrees
void Servo_2_Angle(float angle)
{
  angle=constrain(angle,0,180);
  angle=map(angle,0,180,500,2500);
  PCA9685_SERVO.setChannelServoPulseDuration(PCA9685_CHANNEL_1,int(angle)+servo_2_offset);
}

//Set servo 1 offset
void Set_Servo_1_Offset(int offset)
{
  servo_1_offset=offset;
}

//Set servo 2 offset
void Set_Servo_2_Offset(int offset)
{
  servo_2_offset=offset;
}
