#ifndef _FREENOVE_4WD_CAR_H
#define _FREENOVE_4WD_CAR_H

#include <Arduino.h>
#include <PCA9685.h>

void PCA9685_Close_Com_Address(void);//Close the PCA9685 public address

/////////////////////Servo drive area///////////////////////////////////
#define PCA9685_SERVO_SDA 13         //Define SDA pins
#define PCA9685_SERVO_SCL 14         //Define SCL pins
#ifndef PCA9685_ADDRESS
#define PCA9685_ADDRESS   0x5F
#endif
#define SERVO_FREQUENCY    50        //Define the operating frequency of servo
#define PCA9685_CHANNEL_0  0         //Define PCA9685 channel to control servo 1
#define PCA9685_CHANNEL_1  1         //Define the PCA9685 channel to control servo 2
#define SERVO_MIDDLE_POINT 1500      //Define the middle position of the servo   
extern int servo_1_offset;           //Define the offset variable for servo 1
extern int servo_2_offset;           //Define the offset variable for servo 2
extern PCA9685 PCA9685_SERVO;        //Instantiate a PCA9685 object

void Servo_Setup(void);              //servo initialization
void Servo_1_Angle(float angle);     //Set the rotation parameters of servo 1, and the parameters are 0-180 degrees
void Servo_2_Angle(float angle);     //Set the rotation parameters of servo 2, and the parameters are 0-180 degrees
void Set_Servo_1_Offset(int offset); //Set servo 1 offset
void Set_Servo_2_Offset(int offset); //Set servo 2 offset
//Servo sweep function; id = 1: angle_start: 0 - 180; angle_end: 0 - 180; id = 2: angle_start: 90 - 150; angle_end: 90 - 150
void Servo_Sweep(int servo_id, int angle_start, int angle_end);

/////////////////////Motor drive area///////////////////////////////////
//Motor pin definition
#define PCA9685_MOTOR_SDA 13         //Define the SDA pin number
#define PCA9685_MOTOR_SCL 14         //Define the SCL pin number
#ifndef PCA9685_ADDRESS
#define PCA9685_ADDRESS   0x5F
#endif
#define MOTOR_FREQUENCY   1000       //PWM Frequency
#define MOTOR_SPEED_MIN   -4095      //Define a minimum speed limit for wheels
#define MOTOR_SPEED_MAX   4095       //Define a maximum speed limit for wheels
#define PIN_MOTOR_M1_IN1  15         //Define the positive pole of M1
#define PIN_MOTOR_M1_IN2  14         //Define the negative pole of M1
#define PIN_MOTOR_M2_IN1  9          //Define the positive pole of M2
#define PIN_MOTOR_M2_IN2  8          //Define the negative pole of M2
#define PIN_MOTOR_M3_IN1  12         //Define the positive pole of M3
#define PIN_MOTOR_M3_IN2  13         //Define the negative pole of M3
#define PIN_MOTOR_M4_IN1  10         //Define the positive pole of M4
#define PIN_MOTOR_M4_IN2  11         //Define the negative pole of M4

extern PCA9685 PCA9685_MOTOR;        //Instantiate a PCA9685 object to control the motor
void Motor_Setup(void);              //The initialization function for PCA9685
void Motor_Move(int m1_speed, int m2_speed, int m3_speed, int m4_speed);//A function to control the car motor

//////////////////////Buzzer drive area///////////////////////////////////
//Buzzer pin definition             
#define PIN_BUZZER 2                    //Define the pins for the ESP32 control buzzer
#define BUZZER_CHN 0                    //Define the PWM channel for ESP32
#define BUZZER_FREQUENCY 2000           //Define the resonant frequency of the buzzer 

void Buzzer_Setup(void);                //Buzzer initialization
void Buzzer_Alert(int beat, int rebeat);//Buzzer alarm function

////////////////////Battery drive area/////////////////////////////////////
#define PIN_BATTERY        32        //Set the battery detection voltage pin
#define LOW_VOLTAGE_VALUE  2100      //Set the minimum battery voltage

extern float batteryCoefficient;    //Set the proportional coefficient

int Get_Battery_Voltage_ADC(void);   //Gets the battery ADC value
float Get_Battery_Voltage(void);     //Get the battery voltage value
void Set_Battery_Coefficient(float coefficient);//Set the partial pressure coefficient


#endif
