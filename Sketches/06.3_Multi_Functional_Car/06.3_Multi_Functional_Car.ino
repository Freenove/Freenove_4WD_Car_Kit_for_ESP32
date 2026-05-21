/**********************************************************************
  Filename    : Camera Car
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2024/08/12
**********************************************************************/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "esp_camera.h"
#include "Freenove_4WD_Car_WiFi.h"
#include "Freenove_4WD_Car_Emotion.h"
#include "Freenove_4WD_Car_WS2812.h"
#include "Freenove_4WD_Car_For_ESP32.h"

String CmdArray[8];
int paramters[8];
bool videoFlag = 0;

void WiFi_Init() {
  ssid_Router     =   "********";    //Modify according to your router name
  password_Router =   "********";    //Modify according to your router password
  ssid_AP         =   "Sunshine";    //ESP32 turns on an AP and calls it Sunshine
  password_AP     =   "Sunshine";    //Set your AP password for ESP32 to Sunshine
}

WiFiServer server_Cmd(4000);
WiFiServer server_Camera(7000);

void setup() {
  Buzzer_Setup();           //Buzzer initialization
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  WiFi_Init();              //WiFi paramters initialization
  WiFi_Setup(1);            //Start AP Mode. If you want to connect to a router, change 1 to 0.
  server_Cmd.begin(4000);   //Start the command server
  server_Camera.begin(7000);//Turn on the camera server

  cameraSetup();            //Camera initialization
  Emotion_Setup();          //Emotion initialization
  WS2812_Setup();           //WS2812 initialization
  PCA9685_Setup();          //PCA9685 initialization
  Light_Setup();            //Light initialization
  Track_Setup();            //Track initialization

  Setup_Battery_Monitor();

  xTaskCreateUniversal(loopTask_Camera, "loopTask_Camera", 8192, NULL, 0, NULL, 0);
  xTaskCreateUniversal(loopTask_WTD, "loopTask_WTD", 8192, NULL, 0, NULL, 0);
}

void loop() {
  WiFiClient client = server_Cmd.accept();                 //listen for incoming clients
  if (client) {                                               //if you get a client
    Serial.println("Cmd_Server connected to a client.");
    while (client.connected()) {                              //loop while the client's connected
      if (client.available()) {                               //if there's bytes to read from the client
        String inputStringTemp = client.readStringUntil('\n');//Read the command by WiFi
        Serial.println(inputStringTemp);                      //Print out the command received by WiFi
        Get_Command(inputStringTemp);

        if (CmdArray[0] == CMD_LED_MOD)//Set the display mode of car colored lights
          WS2812_SetMode(paramters[1]);
        if (CmdArray[0] == CMD_LED) //Set the color and brightness of the car lights
          WS2812_Set_Color_1(paramters[1], paramters[2], paramters[3], paramters[4]);
        if (CmdArray[0] == CMD_MATRIX_MOD)//Set the display mode of the LED matrix
          Emotion_SetMode(paramters[1]);
        if (CmdArray[0] == CMD_VIDEO)//Video transmission command
          videoFlag = paramters[1];
        if (CmdArray[0] == CMD_BUZZER) //Buzzer control command
          Buzzer_Variable(paramters[1], paramters[2]);
        if (CmdArray[0] == CMD_POWER) {//Power query command
          float battery_voltage = Get_Battery_Voltage();
          client.print(CMD_POWER);
          client.print(INTERVAL_CHAR);
          client.print(battery_voltage);
          client.print(ENTER);
        }
        if (CmdArray[0] == CMD_MOTOR) {//Network control car movement command
          Car_SetMode(0);
          if (paramters[1] == 0 && paramters[3] == 0)
            Motor_Move(0, 0, 0, 0);//Stop the car
          else //If the parameters are not equal to 0
            Motor_Move(paramters[1], paramters[1], paramters[3], paramters[3]);
        }
        if (CmdArray[0] == CMD_SERVO) {//Network control servo motor movement command
          if (paramters[1] == 0)
            Servo_1_Angle(paramters[2]);
          else if (paramters[1] == 1)
            Servo_2_Angle(paramters[2]);
        }
        if (CmdArray[0] == CMD_CAMERA) {//Network control servo motor movement command
          Servo_1_Angle(paramters[1]);
          Servo_2_Angle(paramters[2]);
        }
        if (CmdArray[0] == CMD_LIGHT) { //Light seeking car command
          if (paramters[1] == 1)
            Car_SetMode(1);
          else if (paramters[1] == 0)
            Car_SetMode(0);
        }
        else if (CmdArray[0] == CMD_TRACK) { //Tracking car command
          if (paramters[1] == 1)
            Car_SetMode(2);
          else if (paramters[1] == 0)
            Car_SetMode(0);
        }
        if (CmdArray[0] == CMD_CAR_MODE) { //Car command Mode
          Car_SetMode(paramters[1]);
        }
        //Clears the command array and parameter array
        memset(CmdArray, 0, sizeof(CmdArray));
        memset(paramters, 0, sizeof(paramters));
      }
      Emotion_Show(emotion_task_mode);//Led matrix display function
      WS2812_Show(ws2812_task_mode);  //Car color lights display function
      Car_Select(carFlag);            //ESP32 Car mode selection function
    }
    client.stop();//close the connection:
    Serial.println("Command Client Disconnected.");
    ESP.restart();
  }
}

void loopTask_Camera(void *pvParameters) {
  while (1) {
    WiFiClient client = server_Camera.accept();//listen for incoming clients
    if (client) {//if you get a client
      Serial.println("Camera_Server connected to a client.");
      if (client.connected()) {
        while (client.connected()) {//loop while the client's connected
          if (videoFlag) { 
            camera_fb_t* fb = esp_camera_fb_get();
            size_t jpg_buf_len = 0;
            uint8_t *jpg_buf = NULL;
            if (fb != NULL) {
              if (fb->format != PIXFORMAT_JPEG){
                  bool jpeg_converted = frame2jpg(fb, 80, &jpg_buf, &jpg_buf_len);
                  esp_camera_fb_return(fb);
                  fb = NULL;
                  if (!jpeg_converted) {
                      Serial.println("JPEG compression failed");
                      continue; 
                  }
              }
              else {
                  jpg_buf_len = fb->len;
                  jpg_buf = fb->buf;
              }
              uint8_t slen[4];
              slen[0] = jpg_buf_len >> 0;
              slen[1] = jpg_buf_len >> 8;
              slen[2] = jpg_buf_len >> 16;
              slen[3] = jpg_buf_len >> 24;
              client.write(slen, 4);
              client.write(jpg_buf, jpg_buf_len);
            } else {
              Serial.println("Failed to get frame");
            }
            if (fb){
                esp_camera_fb_return(fb);
                fb = NULL;
                jpg_buf = NULL;
            }
            else if (jpg_buf){
                free(jpg_buf);
                jpg_buf = NULL;
            }
          } else {
              delay(100); 
          }
        }
        //close the connection:
        client.stop();
        Serial.println("Camera Client Disconnected.");
        ESP.restart();
      }
    }
  }
}

void Get_Command(String inputStringTemp)
{
  int string_length = inputStringTemp.length();
  for (int i = 0; i < 8; i++) {//Parse the command received by WiFi
    int index = inputStringTemp.indexOf(INTERVAL_CHAR);
    if (index < 0) {
      if (string_length > 0) {
        CmdArray[i] = inputStringTemp;         //Get command
        paramters[i] = inputStringTemp.toInt();//Get parameters
      }
      break;
    }
    else {
      string_length -= index;                                //Count the remaining words
      CmdArray[i] = inputStringTemp.substring(0, index);     //Get command
      paramters[i] = CmdArray[i].toInt();                    //Get parameters
      inputStringTemp = inputStringTemp.substring(index + 1);//Update string
    }
  }
}