/**********************************************************************
  Filename    : Camera Tcp Serrver
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2021/12/02
**********************************************************************/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char* ssid_Router     =   "********";   //Modify according to your router name
const char* password_Router =   "********";   //Modify according to your router password
const char* ssid_AP         =   "Sunshine";   //ESP32 turns on an AP and calls it Sunshine
const char* password_AP     =   "Sunshine";   //Set your AP password for ESP32 to Sunshine
WiFiServer server_Cmd(4000);
WiFiServer server_Camera(7000);

void setup() {
  Serial.begin(115200);
  Serial.println();
  WiFi.softAP(ssid_AP, password_AP);//Turn on ESP32's AP feature
  server_Camera.begin(7000);        //Turn on camera server
  server_Cmd.begin(4000);           //Turn on Cmd server
  server_Camera.setNoDelay(true);   //Set no delay in sending and receiving data
  server_Cmd.setNoDelay(true);      //Set no delay in sending and receiving data
  //////////////////////////////////////////////////////////
  WiFi.disconnect(true);
  WiFi.begin(ssid_Router, password_Router);//Make a connector request to the router
  Serial.print("Connecting ");
  Serial.print(ssid_Router);
  int timeout=0;
  while (WiFi.isConnected() != true) { //If the connection fails, wait half a second for another connection request
    delay(500);
    Serial.print(".");
    timeout++;
    //WiFi.begin(ssid_Router, password_Router);
    if(timeout==20)
      break;
  }
  //////////////////////////////////////////////////////////
  Serial.print("\nCamera Ready! Use '");
  Serial.print(WiFi.softAPIP());
  Serial.print(" or ");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect in Freenove app.");
}

void loop() {
  WiFiClient client = server_Cmd.available();//listen for incoming clients
  if (client)                                //if you get a client,
  {
    Serial.println("Command Server connected to a client.");
    while (client.connected()) {//loop while the client's connected
      if (client.available()) { //if there's bytes to read from the client,
        String dataBuffer = client.readStringUntil('\n') + String("\n"); //read data
        Serial.print(dataBuffer);//print it out the serial monitor
      }
    }
    client.stop();// close the connection:
    Serial.println("Command Client Disconnected.");
  }
}
