/**********************************************************************
  Filename    : Camera Tcp Serrver
  Product     : Freenove 4WD Car for ESP32
  Auther      : www.freenove.com
  Modification: 2025/09/29
**********************************************************************/
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#include "camera_pins.h" 

// ============== Mode selection ==============
// MODE_AP
// MODE_STA
#define WIFI_MODE MODE_AP
// ============================================

// --- AP ---
const char* ssid_AP          = "Sunshine";      // ESP32 turns on an AP and calls it Sunshine
const char* password_AP      = "Sunshine";      // Set your AP password for ESP32 to Sunshine

// --- STA ---
const char* ssid_Router      = "********";      // Modify according to your router name
const char* password_Router  = "********";      // Modify according to your router password

bool videoFlag = false;

WiFiServer server_Cmd(4000);
WiFiServer server_Camera(7000);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Camera Stream Application (Switchable Mode)...");

  if (!cameraSetup()) {
    Serial.println("Camera Init Failed. Halting.");
    while (true) { delay(1000); }
  }

  // Initialize based on the selected mode
  if (WIFI_MODE == MODE_AP) {
    WiFi_Setup_AP();
  } else if (WIFI_MODE == MODE_STA) {
    WiFi_Setup_STA();
  } else {
    Serial.println("!!! ERROR: Invalid WIFI_MODE defined. Halting. !!!");
    Serial.println("Please set WIFI_MODE to either MODE_AP or MODE_STA.");
    while(true);
  }

  server_Cmd.begin();
  server_Camera.begin();
  Serial.println("\nCommand and Camera servers started.");
  Serial.println("Ready for App connection.");

  xTaskCreateUniversal(
    loopTask_Camera, "loopTask_Camera", 8192, NULL, 1, NULL, 0
  );
}

void loop() {
  WiFiClient client = server_Cmd.accept();

  if (client) {
    Serial.printf("Command client connected from: %s\n", client.remoteIP().toString().c_str());
    while (client.connected()) {
      if (client.available()) {
        String command = client.readStringUntil('\n');
        command.trim(); 
        
        Serial.print("Received Command: ");
        Serial.println(command);

        if (command.startsWith("CMD_VIDEO")) {
          int separatorIndex = command.indexOf('#');
          if (separatorIndex > 0) {
            String param = command.substring(separatorIndex + 1);
            videoFlag = (param.toInt() == 1);
            if(videoFlag) Serial.println("Video stream STARTED.");
            else Serial.println("Video stream STOPPED.");
          }
        }
      }
      delay(10); 
    }
    client.stop();
    Serial.println("Command client disconnected.");
    videoFlag = false; 
  }
}

//Initialize camera
bool cameraSetup(void) {
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
  config.xclk_freq_hz = 10000000;
  config.frame_size = FRAMESIZE_HQVGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    if(err==ESP_ERR_NOT_SUPPORTED){
      config.pixel_format = PIXFORMAT_RGB565;
      esp_err_t err = esp_camera_init(&config);
      if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return false;
      }
    }
  }

  sensor_t * s = esp_camera_sensor_get();
  // drop down frame size for higher initial frame rate
  uint16_t pid = s->id.PID;
  if(pid == OV2640_PID){
    s->set_hmirror(s, 1);
    s->set_vflip(s, 1);     
  }
  else if(pid == OV3660_PID){
    s->set_hmirror(s, 1);
    s->set_vflip(s, 0);     
  }
  else if(pid == GC2145_PID){
    s->set_hmirror(s, 0);
    delay(500);
    s->set_vflip(s, 1);      
  }
  else if(pid == GC0308_PID){
    s->set_hmirror(s, 0);
    delay(500);
    s->set_vflip(s, 1);     
  }
  else{
    s->set_hmirror(s, 1);
    s->set_vflip(s, 1);       
  }
  s->set_brightness(s, 1);  // Slightly increase brightness
  s->set_saturation(s, 0);  // Reduce saturation
  s->set_ae_level(s, -3);   // Set exposure compensation level

  Serial.println("Camera initialization complete!");
  return true;
}

//Initialize and start ESP32 WiFi in AP mode
void WiFi_Setup_AP(void) {
  Serial.println("\nConfiguring WiFi in AP (Hotspot) Mode...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid_AP, password_AP);
  
  Serial.println("--- WiFi AP Started ---");
  Serial.print("SSID: "); Serial.println(ssid_AP);
  Serial.print("Password: "); Serial.println(password_AP);
  Serial.print("Connect your App to this IP Address: "); 
  Serial.println(WiFi.softAPIP());
}

//Initialize and start ESP32 WiFi in STA mode
void WiFi_Setup_STA(void) {
  Serial.println("\nConfiguring WiFi in STA (Router Connection) Mode...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_Router, password_Router);

  Serial.printf("Attempting to connect to router: %s\n", ssid_Router);
  int timeout = 20;
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    Serial.print(".");
    delay(500);
    timeout--;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n--- WiFi STA Connected ---");
    Serial.print("Connected successfully to router!");
    Serial.print("Connect your App to this IP Address: "); 
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n!!! FAILED to connect to router. !!!");
    Serial.println("Please check your ssid_Router and password_Router settings.");
    Serial.println("Halting execution.");
    while(true);
  }
}

void loopTask_Camera(void *pvParameters) {
  while (1) {
    WiFiClient client = server_Camera.accept();
    if (client) {
      Serial.printf("Camera client connected from: %s\n", client.remoteIP().toString().c_str());
      
      while (client.connected()) {
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
      client.stop();
      Serial.println("Camera client disconnected.");
      videoFlag = false; 
    }
    delay(1); 
  }
}