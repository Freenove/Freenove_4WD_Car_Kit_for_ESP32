/**********************************************************************
  Filename    : IR_Receiver_Car.ino
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2020/12/18
**********************************************************************/

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "Freenove_4WD_Car_For_ESP32.h"

#define RECV_PIN     0        // Infrared receiving pin
IRrecv irrecv(RECV_PIN);      // Create a class object used to receive class
decode_results results;       // Create a decoding results class object

int motor_speed = 3000;
int emotionMode = 4;
int motor_flag = 0;

void setup()
{
  irrecv.enableIRIn();        //Initialize the infrared receiving pin
  PCA9685_Setup();            //Initializes the motor control chip
  Emotion_Setup();
}

void loop()
{
  if (irrecv.decode(&results)) {  // Waiting for decoding
    handleControl(results.value); // Handle the commands from remote control
    irrecv.resume();              // Release infrared decoding function
  }
  showEmotion(emotionMode);
}

void handleControl(unsigned long value) {
  // Handle the commands
  switch (value) {
    case 0xFF02FD:// Receive the number '+'
    case 0xD7E84B1B:
      motor_flag = 1;
      Motor_Move(motor_speed, motor_speed, motor_speed, motor_speed);     // Go forward
      delay(200);
      Motor_Move(0, 0, 0, 0);
      break;

    case 0xFF9867:// Receive the number '-'
    case 0x97483BFB:
      motor_flag = 2;
      Motor_Move(-motor_speed, -motor_speed, -motor_speed, -motor_speed); // Back up
      delay(200);
      Motor_Move(0, 0, 0, 0);
      break;

    case 0xFFE01F:// Receive the number '|<<'
    case 0xF076C13B:
      motor_flag = 3;
      Motor_Move(-motor_speed, -motor_speed, motor_speed, motor_speed);   // Turn left
      delay(200);
      Motor_Move(0, 0, 0, 0);
      break;

    case 0xFF906F:// Receive the number '>>|'
    case 0xE5CFBD7F:
      motor_flag = 4;
      Motor_Move(motor_speed, motor_speed, -motor_speed, -motor_speed);   // Turn right
      delay(200);
      Motor_Move(0, 0, 0, 0);
      break;

    case 0xFF6897:// Receive the number '0'
    case 0xC101E57B:
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
