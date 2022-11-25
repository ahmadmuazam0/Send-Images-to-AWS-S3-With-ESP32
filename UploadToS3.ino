#include "esp_http_client.h"        //// Initialize the HTTP client for esp
#include "esp_camera.h"            //// Initialize the camera 
#include <WiFi.h>                 //// Initialize the WiFi in ESP32
#include "Arduino.h"
#include "Base64.h"              //// COnvert the image to base 64
#include "mbedtls/base64.h"
#include <NTPClient.h>          /// To get real time from the server
#include <WiFiUdp.h>
#include "imagetoAWSs3.h"

int triger=0;       // Variable to sense the single input
/*========================================================
                  Variables to be Updated
  ========================================================*/
// Change the WIFI name and password

const char* ssid = "Epazz2FOffice4-2G";
const char* password = "epazzlahore";

// Microseconds between captures whne event occurs
const int capture_interval = 10000; 
//  Number of pictures to be taken when event occours
int capture_count = 5;        
#define eventtriger 12      // It's IO12 pin of ESP32-CAm module




bool internet_connected = false;
unsigned int past_time = 0;

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
bool init_wifi()
{
  int connAttempts = 0;
  Serial.println("\r\nConnecting to: " + String(ssid));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
    if (connAttempts > 10) return false;
    connAttempts++;
  }
  return true;
}


void setup()
{
  Serial.begin(115200);
  pinMode(eventtriger,INPUT_PULLUP);      // It gives value when drived Low
  
  if (init_wifi()) { // Connected to WiFi
    internet_connected = true;
    Serial.println("Internet connected");
  }

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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void loop()
{
  ///////////////////Check if the event Occours////////////////////////////
  if(eventtriger==LOW){
    triger=1;
  }else triger=0;

 do{

    if(millis() - past_time >= capture_interval){
    take_send_photo();  //// The function that Takes and upload the image to the server AWS S3
    past_time = millis();;
    triger++;}
  } while(triger<=capture_count-1);

triger =0;
}