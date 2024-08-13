/**********************************************************************
  Filename    : IR_Receiver_Car.ino
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2024/08/12
**********************************************************************/

#include <Arduino.h>
#include "Freenove_IR_Lib_for_ESP32.h"
#include "Freenove_4WD_Car_For_ESP32.h"

#define RECV_PIN     0        // Infrared receiving pin
Freenove_ESP32_IR_Recv ir_recv(RECV_PIN);  // Create a class object used to receive class

int motor_speed = 3000;
int emotionMode = 4;
int motor_flag = 0;

void setup()
{
  Serial.begin(115200);
  PCA9685_Setup();            //Initializes the motor control chip
  Emotion_Setup();
}

void loop()
{
  ir_recv.task();                  //Get IR receice data.
  if(ir_recv.nec_available()){     //The data of the NEC protocol is checked
    handleControl(ir_recv.data()); // Handle the commands from remote control
  }
  showEmotion(emotionMode);
}

void handleControl(unsigned long value) {
  // Handle the commands
  Serial.printf("IR Code:  %#x\r\n", value);
  switch (value) {
    case 0xFF02FD:// Receive the number '+'
      motor_flag = 1;
      Motor_Move(motor_speed, motor_speed, motor_speed, motor_speed);     // Go forward
      delay(200);
      Motor_Move(0, 0, 0, 0);
      break;

    case 0xFF9867:// Receive the number '-'
      motor_flag = 2;
      Motor_Move(-motor_speed, -motor_speed, -motor_speed, -motor_speed); // Back up
      delay(200);
      Motor_Move(0, 0, 0, 0);
      break;

    case 0xFFE01F:// Receive the number '|<<'
      motor_flag = 3;
      Motor_Move(-motor_speed, -motor_speed, motor_speed, motor_speed);   // Turn left
      delay(200);
      Motor_Move(0, 0, 0, 0);
      break;

    case 0xFF906F:// Receive the number '>>|'
      motor_flag = 4;
      Motor_Move(motor_speed, motor_speed, -motor_speed, -motor_speed);   // Turn right
      delay(200);
      Motor_Move(0, 0, 0, 0);
      break;

    case 0xFF6897:// Receive the number '0'
      emotionMode = millis() % 10;
      break;

    case 0xFFFFFFFF:// Remain unchanged
      if (motor_flag == 1)
        Motor_Move(motor_speed, motor_speed, motor_speed, motor_speed);     // Go forward
      else if (motor_flag == 2)
        Motor_Move(-motor_speed, -motor_speed, -motor_speed, -motor_speed); // Back up
      else if (motor_flag == 3)
        Motor_Move(-motor_speed, -motor_speed, motor_speed, motor_speed);   // Turn left
      else if (motor_flag == 4)
        Motor_Move(motor_speed, motor_speed, -motor_speed, -motor_speed);   // Turn right
      else
        Motor_Move(0, 0, 0, 0);
      break;

    default:
      motor_flag = 0;
      Motor_Move(0, 0, 0, 0); //stop
      break;
  }
}
