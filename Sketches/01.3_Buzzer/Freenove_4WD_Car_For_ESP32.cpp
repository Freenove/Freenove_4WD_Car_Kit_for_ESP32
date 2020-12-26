#include <Arduino.h>
#include "Freenove_4WD_Car_For_ESP32.h"

/////////////////////Servo drive area///////////////////////////////////
int servo_1_offset = 0; //Define the offset variable for servo 1
int servo_2_offset = 0; //Define the offset variable for servo 2
PCA9685 PCA9685_SERVO;//Instantiate a PCA9685 object

//servo initialization
void Servo_Setup(void)
{
  Wire.begin(PCA9685_SERVO_SDA, PCA9685_SERVO_SCL);
  PCA9685_SERVO.setupSingleDevice(Wire, PCA9685_ADDRESS);
  PCA9685_SERVO.setToFrequency(SERVO_FREQUENCY);
}

//Set the rotation parameters of servo 1, and the parameters are 0-180 degrees
void Servo_1_Angle(float angle)
{
  if (PCA9685_SERVO.getFrequency() != SERVO_FREQUENCY)
    Servo_Setup();
  angle = constrain(angle, 0, 180);
  angle = map(angle, 0, 180, 500, 2500);
  PCA9685_SERVO.setChannelServoPulseDuration(PCA9685_CHANNEL_0, int(angle) + servo_1_offset);
}

//Set the rotation parameters of servo 2, and the parameters are 0-180 degrees
void Servo_2_Angle(float angle)
{
  if (PCA9685_SERVO.getFrequency() != SERVO_FREQUENCY)
    Servo_Setup();
  angle = constrain(angle, 0, 180);
  angle = map(angle, 0, 180, 500, 2500);
  PCA9685_SERVO.setChannelServoPulseDuration(PCA9685_CHANNEL_1, int(angle) + servo_2_offset);
}

//Set servo 1 offset
void Set_Servo_1_Offset(int offset)
{
  servo_1_offset = offset;
}

//Set servo 2 offset
void Set_Servo_2_Offset(int offset)
{
  servo_2_offset = offset;
}

//Servo sweep function
//id = 1: angle_start: 0 - 180; angle_end: 0 - 180.
//id = 2: angle_start: 90 - 150; angle_end: 90 - 150
void Servo_Sweep(int servo_id, int angle_start, int angle_end)
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
      delay(10);
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
      delay(10);
    }
  }
}

/////////////////////Motor drive area///////////////////////////////////
PCA9685 PCA9685_MOTOR;//Instantiate a PCA9685 object to control the motor

//The initialization function for PCA9685
void Motor_Setup(void)
{
  Wire.begin(PCA9685_MOTOR_SDA, PCA9685_MOTOR_SCL);
  PCA9685_MOTOR.setupSingleDevice(Wire, PCA9685_ADDRESS);
  PCA9685_Close_Com_Address();
  PCA9685_MOTOR.setToFrequency(MOTOR_FREQUENCY);
}

//A function to control the car motor
void Motor_Move(int m1_speed, int m2_speed, int m3_speed, int m4_speed)
{
  if (PCA9685_MOTOR.getFrequency() != MOTOR_FREQUENCY)
    Motor_Setup();

  m1_speed = constrain(m1_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  m2_speed = constrain(m2_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  m3_speed = constrain(m3_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
  m4_speed = constrain(m4_speed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);

  if (m1_speed >= 0)
  {
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M1_IN1, m1_speed);
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M1_IN2, 0);
  }
  else
  {
    m1_speed = -m1_speed;
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M1_IN1, 0);
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M1_IN2, m1_speed);
  }
  if (m2_speed >= 0)
  {
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M2_IN1, m2_speed);
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M2_IN2, 0);
  }
  else
  {
    m2_speed = -m2_speed;
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M2_IN1, 0);
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M2_IN2, m2_speed);
  }
  if (m3_speed >= 0)
  {
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M3_IN1, m3_speed);
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M3_IN2, 0);
  }
  else
  {
    m3_speed = -m3_speed;
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M3_IN1, 0);
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M3_IN2, m3_speed);
  }
  if (m4_speed >= 0)
  {
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M4_IN1, m4_speed);
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M4_IN2, 0);
  }
  else
  {
    m4_speed = -m4_speed;
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M4_IN1, 0);
    PCA9685_MOTOR.setChannelPulseWidth(PIN_MOTOR_M4_IN2, m4_speed);
  }
}

void PCA9685_Close_Com_Address(void)
{
  Wire.beginTransmission(PCA9685_ADDRESS);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
}

//////////////////////Buzzer drive area///////////////////////////////////
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
