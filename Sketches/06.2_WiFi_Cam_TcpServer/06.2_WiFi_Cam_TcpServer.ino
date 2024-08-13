/**********************************************************************
  Filename    : Camera Tcp Serrver
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2024/08/12
**********************************************************************/
#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define CAMERA_MODEL_WROVER_KIT
#include "camera_pins.h"

const char* ssid_Router =     "********";      //Modify according to your router name
const char* password_Router = "********";      //Modify according to your router password
const char* ssid_AP =         "Sunshine";      //ESP32 turns on an AP and calls it Sunshine
const char* password_AP =     "Sunshine";      //Set your AP password for ESP32 to Sunshine

WiFiServer server_Cmd(4000);
WiFiServer server_Camera(7000);
extern TaskHandle_t loopTaskHandle;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println();
  cameraSetup();

  WiFi.softAP(ssid_AP, password_AP);
  server_Camera.begin(7000);
  server_Cmd.begin(4000);
  server_Camera.setNoDelay(true);  //Set no delay in sending and receiving data
  server_Cmd.setNoDelay(true);     //Set no delay in sending and receiving data
  /////////////////////////////////////////////////////
  WiFi.begin(ssid_Router, password_Router);
  Serial.print("Connecting ");
  Serial.print(ssid_Router);
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED) {  //If the connection fails, wait half a second for another connection request
    delay(500);
    Serial.print(".");
    timeout++;
    if (timeout == 20)
      break;
  }
  timeout = 0;
  while (WiFi.STA.hasIP() != true) {
    Serial.print(".");
    delay(500);
    timeout++;
    if (timeout == 20)
      break;
  }
  Serial.println("");
  Serial.println("WiFi connected");
  /////////////////////////////////////////////////////
  Serial.print("Camera Ready! Use '");
  Serial.print(WiFi.softAPIP());
  Serial.print(" or ");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect in Freenove app.");

  disableCore0WDT();
  xTaskCreateUniversal(loopTask_Cmd, "loopTask_Cmd", 8192, NULL, 1, &loopTaskHandle, 0);  //loopTask_Cmd uses core 0.
}
//task loop uses core 1.
void loop() {
  WiFiClient client = server_Camera.accept();             // listen for incoming clients
  if (client) {                                              // if you get a client,
    Serial.println("Camera Server connected to a client.");  // print a message out the serial port
    String currentLine = "";                                 // make a String to hold incoming data from the client
    while (client.connected()) {                             // loop while the client's connected
      camera_fb_t* fb = NULL;
      while (client.connected()) {
        fb = esp_camera_fb_get();
        if (fb != NULL) {
          uint8_t slen[4];
          slen[0] = fb->len >> 0;
          slen[1] = fb->len >> 8;
          slen[2] = fb->len >> 16;
          slen[3] = fb->len >> 24;
          client.write(slen, 4);
          client.write(fb->buf, fb->len);
          esp_camera_fb_return(fb);
        } else {
          Serial.println("Camera Error");
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Camera Client Disconnected.");
  }
}

void loopTask_Cmd(void* pvParameters) {
  Serial.println("Task Cmd_Server is starting ... ");
  while (1) {
    WiFiClient client = server_Cmd.accept();  //listen for incoming clients
    if (client)                                  //if you get a client,
    {
      Serial.println("Command Server connected to a client.");
      while (client.connected()) {  //loop while the client's connected
        if (client.available()) {   //if there's bytes to read from the client,
          String dataBuffer = client.readStringUntil('\n') + String("\n");
          Serial.print(dataBuffer);  //print it out the serial monitor
        }
      }
      client.stop();  // close the connection:
      Serial.println("Command Client Disconnected.");
    }
  }
}

void cameraSetup() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //  config.frame_size = FRAMESIZE_VGA;     //640*480
  config.frame_size = FRAMESIZE_CIF;  //400*296
  //  config.frame_size = FRAMESIZE_QVGA;    //320*240
  //  config.frame_size = FRAMESIZE_HQVGA;   //240*176
  //  config.frame_size = FRAMESIZE_QCIF;    //176*144
  //  config.frame_size = FRAMESIZE_QQVGA2;  //128*160
  //  config.frame_size = FRAMESIZE_QQVGA;   //160*120

  config.jpeg_quality = 10;
  config.fb_count = 1;

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  Serial.println("Camera configuration complete!");
}
