/**********************************************************************
  Filename    : IR_Receiver.ino
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2020/12/18
**********************************************************************/
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#define RECV_PIN     0        // Infrared receiving pin
IRrecv irrecv(RECV_PIN);      // Create a class object used to receive class
decode_results results;       // Create a decoding results class object

void setup()
{
  Serial.begin(115200);       // Initialize the serial port and set the baud rate to 115200
  irrecv.enableIRIn();        // Start the receiver
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(RECV_PIN);   //print the infrared receiving pin
}

void loop()
{
  if (irrecv.decode(&results)) {          // Waiting for decoding
    unsigned long value = results.value;
    Serial.println(value, HEX);           // Print out the decoded results
    irrecv.resume();                      // Release the IRremote. Receive the next value
  }
  delay(100);
}
