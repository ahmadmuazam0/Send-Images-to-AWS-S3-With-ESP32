
#include "esp_http_client.h"        //// Initialize the HTTP client for esp
#include "httpeventhandler.h""
#include "esp_camera.h"            //// Initialize the camera 
#include "Base64.h"              //// COnvert the image to base 64
#include "mbedtls/base64.h"
#include <NTPClient.h> 
#include <WiFiUdp.h>
static esp_err_t take_send_photo()
{
  Serial.println("Taking picture...");
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;


 
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return ESP_FAIL;
  }
  
   
  int image_buf_size = 4000 * 1000;                                                  
 uint8_t *image = (uint8_t *)ps_calloc(image_buf_size, sizeof(char));

 size_t length=fb->len;

 size_t olen;

 Serial.print("length is");

 Serial.println(length);
 int err1 = mbedtls_base64_encode(image, image_buf_size, &olen, fb->buf, length);

  esp_http_client_handle_t http_client;
  
  esp_http_client_config_t config_client = {0};

   WiFiUDP ntpUDP;
   NTPClient timeClient(ntpUDP);
   timeClient.begin();
   timeClient.update();
   String Time =  String(timeClient.getEpochTime());
   Serial.print("Time:" );  Serial.print(Time);

   String post_url2 = "https://5czka9sl51.execute-api.eu-central-1.amazon-----------/" + Time + ".jpg"; // Location where images are POSTED
   char post_url3[post_url2.length() + 1];
   post_url2.toCharArray(post_url3, sizeof(post_url3));
  
  config_client.url = post_url3;
  config_client.event_handler = _http_event_handler;
  config_client.method = HTTP_METHOD_PUT;

  http_client = esp_http_client_init(&config_client);

   esp_http_client_set_post_field(http_client, (const char *)fb->buf, fb->len);
  

  esp_http_client_set_header(http_client, "Content-Type", "image/jpg");

  esp_err_t err = esp_http_client_perform(http_client);
  if (err == ESP_OK) {
    Serial.print("esp_http_client_get_status_code: ");
    Serial.println(esp_http_client_get_status_code(http_client));
  }

  esp_http_client_cleanup(http_client);

  esp_camera_fb_return(fb);
}

