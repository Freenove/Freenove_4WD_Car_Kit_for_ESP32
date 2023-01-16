#include <Arduino.h>
#include "Freenove_4WD_Car_For_ESP32.h"
#include "Array.h"
#include "Freenove_VK16K33_Lib_For_ESP32.h"
#include <Wire.h>

/////////////////////PCA9685 drive area///////////////////////////////////
#define PCA9685_SDA 13         //Define SDA pins
#define PCA9685_SCL 14         //Define SCL pins
#ifndef PCA9685_ADDRESS
#define PCA9685_ADDRESS   0x5F
#endif
#define FREQUENCY          1000      //Define the operating frequency of servo
#define PCA9685_CHANNEL_0  0         //Define PCA9685 channel to control servo 1
#define PCA9685_CHANNEL_1  1         //Define the PCA9685 channel to control servo 2
#define SERVO_MIDDLE_POINT 1500      //Define the middle position of the servo   
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

PCA9685 pca9685;//Instantiate a PCA9685 object

//PCA9685 initialization
void PCA9685_Setup(void)
{
  Wire.begin(PCA9685_SDA, PCA9685_SCL);

  Wire.beginTransmission(PCA9685_ADDRESS);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();

  pca9685.setupSingleDevice(Wire, PCA9685_ADDRESS);
  pca9685.setToFrequency(FREQUENCY);
}

//Set the rotation parameters of servo 1, and the parameters are 0-180 degrees
void Servo_1_Angle(float angle)
{
  angle = constrain(angle, 0, 180);
  angle = map(angle, 0, 180, 102, 512);
  pca9685.setChannelPulseWidth(PCA9685_CHANNEL_0, int(angle));
}

//Set the rotation parameters of servo 2, and the parameters are 0-180 degrees
void Servo_2_Angle(float angle)
{
  angle = constrain(angle, 0, 180);
  angle = map(angle, 0, 180, 102, 512);
  pca9685.setChannelPulseWidth(PCA9685_CHANNEL_1, int(angle));
}

//Servo sweep function
void Servo_Sweep(int servo_id, int angle_start, int angle_end, int speed)
{
  if (servo_id == 1)
  {
    angle_start = constrain(angle_start, 0, 180);
    angle_end = constrain(angle_end, 0, 180);
  }
  else if (servo_id == 2)
  {
    angle_start = constrain(angle_start, 90, 150);
    angle_end = constrain(angle_end, 90, 150);
  }

  if (angle_start > angle_end)
  {
    for (int i = angle_start; i >= angle_end; i--)
    {
      if (servo_id == 1)
        Servo_1_Angle(i);
      else if (servo_id == 2)
        Servo_2_Angle(i);
      delay(speed);
    }
  }
  if (angle_start < angle_end)
  {
    for (int i = angle_start; i <= angle_end; i++)
    {
      if (servo_id == 1)
        Servo_1_Angle(i);
      else if (servo_id == 2)
        Servo_2_Angle(i);
      delay(speed);
    }
  }
}

//A function to control the car motor
void Motor_Move(int m1_speed, int m2_speed, int m3_speed, int m4_speed)
{
  m1_speed = MOTOR_1_DIRECTION * constrain(m1_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  m2_speed = MOTOR_2_DIRECTION * constrain(m2_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  m3_speed = MOTOR_3_DIRECTION * constrain(m3_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  m4_speed = MOTOR_4_DIRECTION * constrain(m4_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);

  if (m1_speed >= 0)
  {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN1, m1_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN2, 0);
  }
  else
  {
    m1_speed = -m1_speed;
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M1_IN2, m1_speed);
  }
  if (m2_speed >= 0)
  {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN1, m2_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN2, 0);
  }
  else
  {
    m2_speed = -m2_speed;
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M2_IN2, m2_speed);
  }
  if (m3_speed >= 0)
  {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN1, m3_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN2, 0);
  }
  else
  {
    m3_speed = -m3_speed;
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M3_IN2, m3_speed);
  }
  if (m4_speed >= 0)
  {
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN1, m4_speed);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN2, 0);
  }
  else
  {
    m4_speed = -m4_speed;
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN1, 0);
    pca9685.setChannelPulseWidth(PIN_MOTOR_M4_IN2, m4_speed);
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

//Buzzer alarm function
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
#define LOW_VOLTAGE_VALUE  2100      //Set the minimum battery voltage

float batteryVoltage = 0;       //Battery voltage variable
float batteryCoefficient = 3.4;   //Set the proportional coefficient

//Gets the battery ADC value
int Get_Battery_Voltage_ADC(void)
{
  pinMode(PIN_BATTERY, INPUT);
  int batteryADC = 0;
  for (int i = 0; i < 5; i++)
    batteryADC += analogRead(PIN_BATTERY);
  return batteryADC / 5;
}

//Get the battery voltage value
float Get_Battery_Voltage(void)
{
  int batteryADC = Get_Battery_Voltage_ADC();
  if (batteryADC <= LOW_VOLTAGE_VALUE)
  {
    return 0;
  }
  batteryVoltage = (batteryADC / 4096.0  * 3.3 ) * batteryCoefficient;
  return batteryVoltage;
}

void Set_Battery_Coefficient(float coefficient)
{
  batteryCoefficient = coefficient;
}

/////////////////////Photosensitive drive area//////////////////////////
#define PHOTOSENSITIVE_PIN   33

//Photosensitive initialization
void Photosensitive_Setup(void)
{
  pinMode(PHOTOSENSITIVE_PIN, INPUT);
}

//Gets the photosensitive resistance value
int Get_Photosensitive(void)
{
  int photosensitiveADC = analogRead(PHOTOSENSITIVE_PIN);
  return photosensitiveADC;
}

/////////////////////Ultrasonic drive area//////////////////////////////
#define PIN_SONIC_TRIG    12            //define Trig pin
#define PIN_SONIC_ECHO    15            //define Echo pin
#define MAX_DISTANCE      300           //cm
#define SONIC_TIMEOUT (MAX_DISTANCE*60) // calculate timeout 
#define SOUND_VELOCITY    340           //soundVelocity: 340m/s

//Ultrasonic initialization
void Ultrasonic_Setup(void)
{
  pinMode(PIN_SONIC_TRIG, OUTPUT);// set trigPin to output mode
  pinMode(PIN_SONIC_ECHO, INPUT); // set echoPin to input mode
}

//Obtain ultrasonic distance data
float Get_Sonar(void)
{
  unsigned long pingTime;
  float distance;
  digitalWrite(PIN_SONIC_TRIG, HIGH); // make trigPin output high level lasting for 10μs to triger HC_SR04,
  delayMicroseconds(10);
  digitalWrite(PIN_SONIC_TRIG, LOW);
  pingTime = pulseIn(PIN_SONIC_ECHO, HIGH, SONIC_TIMEOUT); // Wait HC-SR04 returning to the high level and measure out this waitting time
  if (pingTime != 0)
    distance = (float)pingTime * SOUND_VELOCITY / 2 / 10000; // calculate the distance according to the time
  else
    distance = MAX_DISTANCE;
  return distance; // return the distance value
}

/////////////////////PCF8574 drive area//////////////////////////////
#define PCF8574_ADDRESS 0x20     //Tracking module IIC address
#define PCF8574_SDA     13       //Define the SDA pin number
#define PCF8574_SCL     14       //Define the SCL pin number

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

//////////////////Emotion drive area////////////////////////////////
#define EMOTION_ADDRESS 0x71
#define EMOTION_SDA     13
#define EMOTION_SCL     14
Freenove_ESP32_VK16K33 matrix = Freenove_ESP32_VK16K33();
int time_before = 0;    //Record each non-blocking time
int time_count = 0;     //Record the number of non-blocking times
int time_flag = 0;      //Record the blink time

//Initialize
void Emotion_Setup()
{
  matrix.init(EMOTION_ADDRESS, EMOTION_SDA, EMOTION_SCL);
  time_before = millis();
}

//Turn the eyes-1
void eyesRotate(int delay_ms)
{
  int count = sizeof(eyeRotate1) / sizeof(eyeRotate1[0]);
  if (millis() - time_before >= delay_ms)
  {
    matrix.showStaticArray(eyeRotate1[time_count], eyeRotate2[time_count]);
    time_before = millis();
    time_count++;
    if (time_count >= count)
      time_count = 0;
  }
}

//Wink the eyes-2
void eyesBlink(int delay_ms)
{
  int count = sizeof(eyeBlink) / sizeof(eyeBlink[0]);
  if (millis() - time_before >= delay_ms)
  {
    time_before = millis();
    time_count++;
    if (time_count >= 25)
    {
      time_count = 0;
      time_flag = 1;
    }
    if (time_flag == 0)
      matrix.showStaticArray(eyeBlink[0], eyeBlink[0]);
    else if (time_flag == 1)
    {
      matrix.showStaticArray(eyeBlink[time_count], eyeBlink[time_count]);
      if (time_count >= (count - 1))
      {
        time_flag = 0;
        time_count = 0;
      }
    }

  }
}

//Smile-3
void eyesSmile(int delay_ms)
{
  int count = sizeof(eyeSmile) / sizeof(eyeSmile[0]);
  if (millis() - time_before >= delay_ms)
  {
    matrix.showStaticArray(eyeSmile[time_count], eyeSmile[time_count]);
    time_before = millis();
    time_count++;
    if (time_count >= count)
      time_count = 0;
  }
}

//Cry-4
void eyesCry(int delay_ms)
{
  int count = sizeof(eyeCry1) / sizeof(eyeCry1[0]);
  if (millis() - time_before >= delay_ms)
  {
    matrix.showStaticArray(eyeCry1[time_count], eyeCry2[time_count]);
    time_before = millis();
    time_count++;
    if (time_count >= count)
      time_count = 0;
  }
}

//Wink the eyes-5
void eyesBlink1(int delay_ms)
{
  int count = sizeof(eyeBlink1) / sizeof(eyeBlink1[0]);
  if (millis() - time_before >= delay_ms)
  {
    time_before = millis();
    time_count++;
    if (time_count >= 15)
    {
      time_count = 0;
      time_flag = 1;
    }
    if (time_flag == 0)
      matrix.showStaticArray(eyeBlink1[0], eyeBlink1[0]);
    else if (time_flag == 1)
    {
      matrix.showStaticArray(eyeBlink1[time_count], eyeBlink1[time_count]);
      if (time_count >= (count - 1))
      {
        time_flag = 0;
        time_count = 0;
      }
    }
  }
}

//show_arrow-6
void showArrow(int arrow_direction, int delay_ms)
{
  if (arrow_direction == 1)
  {
    if (millis() - time_before >= delay_ms)
    {
      matrix.showLedMatrix(arrow_up, 4, time_count - 8);
      time_before = millis();
      time_count++;
      if (time_count > 16)
        time_count = 0;
    }
  }
  else if (arrow_direction == 2)
  {
    if (millis() - time_before >= delay_ms)
    {
      matrix.showLedMatrix(arrow_dowm, 4, 8 - time_count);
      time_before = millis();
      time_count++;
      if (time_count > 16)
        time_count = 0;
    }
  }
  else if (arrow_direction == 3)
  {
    if (millis() - time_before >= delay_ms)
    {
      matrix.showLedMatrix(arrow_left, 8 - time_count, 0);
      time_before = millis();
      time_count++;
      if (time_count > 8)
        time_count = 0;
    }
  }
  else if (arrow_direction == 4)
  {
    if (millis() - time_before >= delay_ms)
    {
      matrix.showLedMatrix(arrow_right, time_count, 0);
      time_before = millis();
      time_count++;
      if (time_count > 8)
        time_count = 0;
    }
  }
  else
    matrix.clear();
}

//wheel-7
void wheel(int mode, int delay_ms)
{
  if (mode == 1)
  {
    int count = sizeof(wheel_left) / sizeof(wheel_left[0]);
    if (millis() - time_before >= delay_ms)
    {
      matrix.showStaticArray(wheel_left[time_count], wheel_left[time_count]);
      time_before = millis();
      time_count++;
      if (time_count >= count)
        time_count = 0;
    }
  }
  else if (mode == 2)
  {
    int count = sizeof(wheel_right) / sizeof(wheel_right[0]);
    if (millis() - time_before >= delay_ms)
    {
      matrix.showStaticArray(wheel_right[time_count], wheel_right[time_count]);
      time_before = millis();
      time_count++;
      if (time_count >= count)
        time_count = 0;
    }
  }
  else
    matrix.clear();
}

//Car-8
void carMove(int mode, int delay_ms)
{
  if (mode == 1)
  {
    if (millis() - time_before >= delay_ms)
    {
      matrix.showLedMatrix(car_left, 8 - time_count, 0);
      time_before = millis();
      time_count++;
      if (time_count >= 8)
        time_count = 0;
    }
  }
  else if (mode == 2)
  {
    if (millis() - time_before >= delay_ms)
    {
      matrix.showLedMatrix(car_right, time_count, 0);
      time_before = millis();
      time_count++;
      if (time_count >= 8)
        time_count = 0;
    }
  }
  else
    matrix.clear();
}

//expressing love-9
void expressingLove(int delay_ms)
{
  int count = sizeof(I_love_you) / sizeof(I_love_you[0]);
  if (millis() - time_before >= delay_ms)
  {
    matrix.showStaticArray(I_love_you[0], I_love_you[1]);
    time_before = millis();
    time_count++;
    if (time_count >= count)
      time_count = 0;
  }
}

//save water-10
void saveWater(int delay_ms)
{
  int count = sizeof(save_water_left) / sizeof(save_water_left[0]);
  if (millis() - time_before >= delay_ms)
  {
    matrix.showStaticArray(save_water_left[time_count], save_water_right[time_count]);
    time_before = millis();
    time_count++;
    if (time_count >= count)
      time_count = 0;
  }
}

//Random emoticons
void showEmotion(int mode)
{
  if (mode == 0)
    eyesRotate(100);
  else if (mode == 1)
    eyesBlink(100);
  else if (mode == 2)
    eyesSmile(100);
  else if (mode == 3)
    eyesCry(100);
  else if (mode == 4)
    eyesBlink1(100);
  else if (mode == 5)
    wheel(1, 150);
  else if (mode == 6)
    wheel(2, 150);
  else if (mode == 7)
    carMove(1, 150);
  else if (mode == 8)
    carMove(2, 150);
  else if (mode == 9)
    saveWater(100);
  else if (mode == 10)
    expressingLove(100);
}
