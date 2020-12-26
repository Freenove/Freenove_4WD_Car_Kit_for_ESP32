/**********************************************************************
  Product     : Freenove 4WD Car for ESP32
  Description : Use buzzer
  Auther      : www.freenove.com
  Modification: 2020/12/18
**********************************************************************/
#include "Freenove_4WD_Car_For_ESP32.h"

void setup() {
  Buzzer_Setup();    //Buzzer initialization function
  Buzzer_Alert(4, 3);//Control the buzzer to sound 3 times, 4 sounds each time
}

void loop() {
  delay(1000);
}
