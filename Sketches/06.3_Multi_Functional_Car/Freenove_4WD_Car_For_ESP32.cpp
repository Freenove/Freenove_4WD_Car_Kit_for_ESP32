#include <Arduino.h>
#include "Freenove_4WD_Car_For_ESP32.h"
#include <Wire.h>
#include "esp_camera.h"
#include "Freenove_4WD_Car_WiFi.h"
#include "Freenove_4WD_Car_Emotion.h"
#include "Freenove_4WD_Car_WS2812.h"

/////////////////////PCA9685 drive area///////////////////////////////////
PCA9685 pca9685;//Instantiate a PCA9685 object

//PCA9685 initialization
void PCA9685_Setup(void)
{
  Wire.begin(PCA9685_SDA, PCA9685_SCL);
  pca9685.setupSingleDevice(Wire, PCA9685_ADDRESS);
  pca9685.setToFrequency(FREQUENCY);
  Motor_Move(0,0,0,0);
  Servo_1_Angle(90);
  Servo_2_Angle(90);
}

//Set the rotation parameters of servo 1, and the parameters are 0-180 degrees
void Servo_1_Angle(int angle)
{
  float s1_angle = constrain(angle, 0, 180);
  if (s1_angle == 0)
    s1_angle = 102;
  else if (s1_angle == 180)
    s1_angle = 512;
  else
  {
    s1_angle = map(s1_angle, 0, 180, 102, 512);
  }
  pca9685.setChannelPulseWidth(PCA9685_CHANNEL_0, int(s1_angle));
}

//Set the rotation parameters of servo 2, and the parameters are 0-180 degrees
void Servo_2_Angle(int angle)
{
  float s2_angle = constrain(angle, 80, 180);
  if (s2_angle == 0)
    s2_angle = 102;
  else if (s2_angle == 180)
    s2_angle = 512;
  else
  {
    s2_angle = map(s2_angle, 0, 180, 102, 512);
  }
  pca9685.setChannelPulseWidth(PCA9685_CHANNEL_1, int(s2_angle));
}

//A function to control the car motor
void Motor_Move(int m1_speed, int m2_speed, int m3_speed, int m4_speed)
{
  m1_speed = MOTOR_1_DIRECTION * constrain(m1_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  m2_speed = MOTOR_2_DIRECTION * constrain(m2_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  m3_speed = MOTOR_3_DIRECTION * constrain(m3_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  m4_speed = MOTOR_4_DIRECTION * constrain(m4_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  if (m1_speed > 0)
  {
    m1_speed = constrain(m1_speed, 1600, 4095);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN1, m1_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN2, 0);
  }
  else if (m1_speed < 0)
  {
    m1_speed = -m1_speed;
    m1_speed = constrain(m1_speed, 1600, 4095);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN2, m1_speed);
  }
  else
  {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN2, 0);
  }
  if (m2_speed > 0)
  {
    m2_speed = constrain(m2_speed, 1600, 4095);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN1, m2_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN2, 0);
  }
  else if (m2_speed < 0)
  {
    m2_speed = -m2_speed;
    m2_speed = constrain(m2_speed, 1600, 4095);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN2, m2_speed);
  }
  else
  {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN2, 0);
  }
  if (m3_speed > 0)
  {
    m3_speed = constrain(m3_speed, 1600, 4095);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN1, m3_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN2, 0);
  }
  else if (m3_speed < 0)
  {
    m3_speed = -m3_speed;
    m3_speed = constrain(m3_speed, 1600, 4095);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN2, m3_speed);
  }
  else
  {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN2, 0);
  }
  if (m4_speed > 0)
  {
    m4_speed = constrain(m4_speed, 1600, 4095);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN1, m4_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN2, 0);
  }
  else if (m4_speed < 0)
  {
    m4_speed = -m4_speed;
    m4_speed = constrain(m4_speed, 1600, 4095);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN2, m4_speed);
  }
  else
  {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN2, 0);
  }
}

//////////////////////Buzzer drive area///////////////////////////////////
//Buzzer pin definition
#define PIN_BUZZER 2                    //Define the pins for the ESP32 control buzzer
#define BUZZER_CHN 0                    //Define the PWM channel for ESP32
#define BUZZER_FREQUENCY 2000           //Define the resonant frequency of the buzzer 

//Buzzer initialization
void Buzzer_Setup(void)
{
  pinMode(PIN_BUZZER, OUTPUT);
  ledcSetup(BUZZER_CHN, BUZZER_FREQUENCY, 10);
  ledcAttachPin(PIN_BUZZER, BUZZER_CHN);
  ledcWriteTone(BUZZER_CHN, 0);
  delay(10);
}

//Buzzer variable frequency
void Buzzer_Variable(bool enable, int frequency)
{
  if (enable == 1)
  {
    frequency = constrain(frequency, 0, 10000);
    ledcWriteTone(BUZZER_CHN, frequency);
  }
  else
    ledcWriteTone(BUZZER_CHN, 0);
}

//Buzzer alarm function
void Buzzer_Alarm(bool enable)
{
  if (enable == 0)
    ledcWriteTone(BUZZER_CHN, 0);
  else
    ledcWriteTone(BUZZER_CHN, BUZZER_FREQUENCY);
}

//Buzzer Alert function
void Buzzer_Alert(int beat, int rebeat)
{
  beat = constrain(beat, 1, 9);
  rebeat = constrain(rebeat, 1, 255);
  for (int j = 0; j < rebeat; j++)
  {
    for (int i = 0; i < beat; i++)
    {
      ledcWriteTone(BUZZER_CHN, BUZZER_FREQUENCY);
      delay(100);
      ledcWriteTone(BUZZER_CHN, 0);
      delay(100);
    }
    delay(500);
  }
  ledcWriteTone(BUZZER_CHN, 0);
}

////////////////////Battery drive area/////////////////////////////////////
#define PIN_BATTERY        32        //Set the battery detection voltage pin
float batteryVoltage     = 0;        //Battery voltage variable
float batteryCoefficient = 3.7;        //Set the proportional coefficient

//Gets the battery ADC value
int Get_Battery_Voltage_ADC(void)
{
  pinMode(PIN_BATTERY, INPUT);
  int batteryADC = analogRead(PIN_BATTERY);
  pinMode(PIN_BATTERY, OUTPUT);
  return batteryADC;
}

//Get the battery voltage value
float Get_Battery_Voltage(void)
{
  int batteryADC = Get_Battery_Voltage_ADC();
  batteryVoltage = (batteryADC / 4096.0  * 3.9 ) * batteryCoefficient;
  return batteryVoltage;
}

/////////////////////Photosensitive drive area//////////////////////////
#define PHOTOSENSITIVE_PIN   33
int light_init_value = 0;   //Set the car's initial environment ADC value
//Photosensitive initialization
void Light_Setup(void)
{
  pinMode(PHOTOSENSITIVE_PIN, INPUT);
  light_init_value = Get_Photosensitive();   //Set the car's initial environment ADC value
}

//Gets the photosensitive resistance value
int Get_Photosensitive(void)
{
  int photosensitiveADC = analogRead(PHOTOSENSITIVE_PIN);
  return photosensitiveADC;
}

//Light Car
void Light_Car(int mode)
{
  if (mode == 1)
  {
    if (Get_Photosensitive() < (light_init_value - 100))
    {
      Motor_Move(-2000, -2000, 2000, 2000);
      Emotion_SetMode(4);
    }
    else if (Get_Photosensitive() > (light_init_value + 100))
    {
      Motor_Move(2000, 2000, -2000, -2000);
      Emotion_SetMode(5);
    }
    else
    {
      Motor_Move(0, 0, 0, 0);
      Emotion_SetMode(1);
    }
  }
}

/////////////////////Track drive area//////////////////////////////
#define PCF8574_ADDRESS 0x20     //Tracking module IIC address
#define PCF8574_SDA     13       //Define the SDA pin number
#define PCF8574_SCL     14       //Define the SCL pin number
#define SPEED_LV4       (4000)
#define SPEED_LV3       (3000)
#define SPEED_LV2       (2500)
#define SPEED_LV1       (1500)

unsigned char sensorValue[4] = {0};
PCF8574 TRACK_SENSOR(PCF8574_ADDRESS);

//Trace module initialization
void Track_Setup(void)
{
  TRACK_SENSOR.begin(PCF8574_SDA, PCF8574_SCL);
}

//Tracking module reading
void Track_Read(void)
{
  sensorValue[3] = (TRACK_SENSOR.read8() & 0x07);//composite value
  sensorValue[0] = (sensorValue[3] & 0x01) >> 0; //On the left - 1
  sensorValue[1] = (sensorValue[3] & 0x02) >> 1; //In the middle - 2
  sensorValue[2] = (sensorValue[3] & 0x04) >> 2; //On the right - 4
}

//Track Car
void Track_Car(int mode)
{
  if (mode == 1)
  {
    Track_Read();
    switch (sensorValue[3])
    {
      case 2:   //010
      case 5:   //101
        Emotion_SetMode(3);
        Motor_Move(SPEED_LV1, SPEED_LV1, SPEED_LV1, SPEED_LV1);    //Move Forward
        break;
      case 0:   //000
      case 7:   //111
        Emotion_SetMode(6);
        Motor_Move(0, 0, 0, 0);                                    //Stop
        break;
      case 1:   //001
      case 3:   //011
        Emotion_SetMode(4);
        Motor_Move(-SPEED_LV3, -SPEED_LV3, SPEED_LV4, SPEED_LV4);  //Turn Left
        break;
      case 4:   //100
      case 6:   //110
        Emotion_SetMode(5);
        Motor_Move(SPEED_LV4, SPEED_LV4 , - SPEED_LV3, -SPEED_LV3);//Turn Right
        break;

      default:
        break;
    }
  }
}

//////////////////////Car drive area////////////////////////////////////////
int carFlag = 0;
//set car mode
void Car_SetMode(int mode)
{
  carFlag = mode;
}

//select it to run car
void Car_Select(int mode)
{
  if (mode == 1)
  {
    Light_Car(1);
    Track_Car(0);
  }
  else if (mode == 2)
  {
    Light_Car(0);
    Track_Car(1);
  }
  else
  {
    Light_Car(0);
    Track_Car(0);
  }
}







//
