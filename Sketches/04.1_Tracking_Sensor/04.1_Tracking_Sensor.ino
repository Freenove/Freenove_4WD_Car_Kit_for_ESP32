/**********************************************************************
  Filename    : Tracking_Sensor.ino
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2020/12/18
**********************************************************************/
#include <Arduino.h>
#include "Freenove_4WD_Car_For_ESP32.h"

void setup() {
  Serial.begin(115200); //set baud rate
  Track_Setup();
}

void loop() {
  Track_Read();
  Serial.print("Sensor Value (L / M / R / ALL) : ");
  for (int i = 0; i < 4; i++) {
    Serial.print(sensorValue[i]);
    Serial.print('\t');
  }
  Serial.print('\n');
  delay(500);
}
