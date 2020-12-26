#ifndef _FREENOVE_4WD_CAR_H
#define _FREENOVE_4WD_CAR_H

#include <Arduino.h>
#include <PCA9685.h>

/////////////////////Servo drive area///////////////////////////////////
#define PCA9685_SERVO_SDA 13                    //Define SDA pins
#define PCA9685_SERVO_SCL 14                    //Define SCL pins

#ifndef PCA9685_ADDRESS
#define PCA9685_ADDRESS   0x5F
#endif

#define SERVO_FREQUENCY    50                   //Define the operating frequency of servo
#define PCA9685_CHANNEL_0  0                    //Define PCA9685 channel to control servo 1
#define PCA9685_CHANNEL_1  1                    //Define the PCA9685 channel to control servo 2
#define SERVO_MIDDLE_POINT 1500                 //Define the middle position of the servo   

extern int servo_1_offset;                      //Define the offset variable for servo 1
extern int servo_2_offset;                      //Define the offset variable for servo 2

extern PCA9685 PCA9685_SERVO;                   //Instantiate a PCA9685 object

void Servo_Setup(void);                         //servo initialization
void Servo_1_Angle(float angle);                //Set the rotation parameters of servo 1, and the parameters are 0-180 degrees
void Servo_2_Angle(float angle);                //Set the rotation parameters of servo 2, and the parameters are 0-180 degrees
void Set_Servo_1_Offset(int offset);            //Set servo 1 offset
void Set_Servo_2_Offset(int offset);            //Set servo 2 offset

#endif
