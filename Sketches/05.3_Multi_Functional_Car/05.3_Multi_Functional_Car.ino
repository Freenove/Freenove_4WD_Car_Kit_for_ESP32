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
#include "Freenove_4WD_Car_Emotion.h"
#include "Freenove_WS2812_Lib_for_ESP32.h"

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(12, 32, 0, TYPE_GRB);
u8 m_color[5][3] = { {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 255}, {0, 0, 0} };


#define RECV_PIN     0        // Infrared receiving pin
IRrecv irrecv(RECV_PIN);      // Create a class object used to receive class
decode_results results;       // Create a decoding results class object

static int servo_1_angle=90;
static int servo_2_angle=90;
static int emotion_flag=0;
static int ws2812_flag=0;

void setup()
{
  Serial.begin(115200);
  PCA9685_Setup();
  Buzzer_Setup();
  Emotion_Setup();
  strip.begin();
  strip.setBrightness(10);  
  
  irrecv.enableIRIn();            //Initialize the infrared receiving pin
  Servo_1_Angle(servo_1_angle);
  Servo_2_Angle(servo_2_angle);
}

void loop()
{
  if (irrecv.decode(&results)) {  // Waiting for decoding
    handleControl(results.value); // Handle the commands from remote control
    unsigned long value=results.value;
    Serial.print(value,HEX);
    Serial.println();
    irrecv.resume();              // Release infrared decoding function
  }
}

void handleControl(unsigned long value) 
{
  // Handle the commands
  switch (value) {
    case 0xFF02FD:// Receive the number '+'
    case 0xD7E84B1B:
      Motor_Move(2000,2000,2000,2000);
      break;
    case 0xFF9867:// Receive the number '-'
    case 0x97483BFB:
      Motor_Move(-2000,-2000,-2000,-2000);
      break;
    case 0xFFE01F:// Receive the number '|<<'
    case 0xF076C13B:
      Motor_Move(-2000,-2000,2000,2000);
      delay(200);
      Motor_Move(0,0,0,0);
      break;
    case 0xFF906F:// Receive the number '>>|'
    case 0xE5CFBD7F:
      Motor_Move(2000,2000,-2000,-2000);
      delay(200);
      Motor_Move(0,0,0,0);      
      break;
    case 0xFFA857:// Receive the number '▶'
    case 0xA3C8EDDB:
      Motor_Move(0,0,0,0);
      break;
    case 0xFF6897:// Receive the number '0'
    case 0xC101E57B:
      servo_1_angle=servo_1_angle+10;
      Servo_1_Angle(servo_1_angle);
      break;
    case 0xFF30CF:// Receive the number '1'
    case 0x9716BE3F:
      servo_1_angle=servo_1_angle-10;
      Servo_1_Angle(servo_1_angle);
      break;   
    case 0xFF10EF:// Receive the number '4'
    case 0x8C22657B:
      servo_1_angle=90;
      Servo_1_Angle(servo_1_angle);
      break; 
    case 0xFFB04F:// Receive the number 'C'
    case 0xF0C41643:
      servo_2_angle=servo_2_angle+10;
      Servo_2_Angle(servo_2_angle);
      break;      
    case 0xFF7A85:// Receive the number '3'
    case 0x6182021B:
      servo_2_angle=servo_2_angle-10;
      Servo_2_Angle(servo_2_angle);
      break;
    case 0xFF5AA5:// Receive the number '6'
    case 0x449E79F:
      servo_2_angle=90;
      Servo_2_Angle(servo_2_angle);
      break;      
    case 0xFF22DD:// Receive the number 'TEST'
    case 0x52A3D41F:
      Buzzer_Alert(1,1);
      break;
    case 0xFF18E7:// Receive the number '2'
    case 0x3D9AE3F7:
      emotion_flag=millis()%21;
      staticEmtions(emotion_flag);
      break;
    case 0xFF38C7:// Receive the number '5'
    case 0x488F3CBB:
      clearEmtions();
      break;
    case 0xFF42BD:// Receive the number '7'
    case 0x32C6FDF7:
      ws2812_flag=millis()%4;
      WS2812_Show();
      break;
    case 0xFF4AB5:// Receive the number '8'
    case 0x1BC0157B:
      ws2812_flag=4;
      WS2812_Show();
      break; 
    case 0xFF52AD:// Receive the number '9'
    case 0x3EC3FC1B:
      break;  
    case 0xFFFFFFFF:// Remain unchanged
      break;
    default:
      break;
  }
}

void WS2812_Show()
{
  for (int i = 0; i < 12; i++)
    strip.setLedColorData(i, m_color[ws2812_flag][0], m_color[ws2812_flag][1], m_color[ws2812_flag][2]);
    strip.show();
}














//
