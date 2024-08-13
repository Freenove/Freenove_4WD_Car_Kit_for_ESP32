/**********************************************************************
  Filename    : IR_Receiver.ino
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2024/08/12
**********************************************************************/
#include <Arduino.h>
#include "Freenove_IR_Lib_for_ESP32.h"

#define RECV_PIN     0                     // Infrared receiving pin
Freenove_ESP32_IR_Recv ir_recv(RECV_PIN);  // Create a class object used to receive class

void setup()
{
  Serial.begin(115200);       // Initialize the serial port and set the baud rate to 115200
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(RECV_PIN);   //print the infrared receiving pin
}

void loop()
{
  ir_recv.task();                  //Get IR receice data.
  if(ir_recv.nec_available()){     //The data of the NEC protocol is checked
    //Print IR data.
    Serial.printf("IR Code:  %#x\r\n", ir_recv.data());
  }
  delay(100);
}
