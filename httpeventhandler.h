#include "esp_http_client.h"        //// Initialize the HTTP client for esp
esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
  switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
      Serial.println("HTTP_EVENT_ERROR");
      break;
    case HTTP_EVENT_ON_CONNECTED:
      Serial.println("HTTP_EVENT_ON_CONNECTED");
      break;
    case HTTP_EVENT_HEADER_SENT:
      Serial.println("HTTP_EVENT_HEADER_SENT");
      break;
    case HTTP_EVENT_ON_HEADER:
      Serial.println();
      Serial.printf("HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
      break;
    case HTTP_EVENT_ON_DATA:
      Serial.println();
      Serial.printf("HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
      if (!esp_http_client_is_chunked_response(evt->client)) {
        // Write out data
        // printf("%.*s", evt->data_len, (char*)evt->data);
      }
      break;
    case HTTP_EVENT_ON_FINISH:
      Serial.println("");
      Serial.println("HTTP_EVENT_ON_FINISH");
      break;
    case HTTP_EVENT_DISCONNECTED:
      Serial.println("HTTP_EVENT_DISCONNECTED");
      break;
  }
  return ESP_OK;
}