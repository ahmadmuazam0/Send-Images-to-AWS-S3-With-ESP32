# Send-Images-to-AWS-S3-With-ESP32
Capture Images with ESP32 cam and send them to S3 with required interval and required number of Snapshot

Things to be changed are
First change the SSID and password for esp32 to be connected in main file "UploadtoS3".
Line 17: const char* ssid = "**********";
Line 18: const char* password = "******";

Second thing is the API adress to send the data to the AWS S3 server in file "imagetoAWSs3.h"

Change line 47: 
String post_url2 = "https://5czka9sl51.execute-api.eu-central-1.amazon-----------/" + Time + ".jpg";

===============================================================================================================================

To chnage the number of Image capture update the value at Line 23:
capture_count = 

To chnage the interval of capture images, update the value at Line 21:
capture_interval = 

To change the pin number for trigerring event at line 
#define eventtriger =12 to your desired number.



Make sure you have these libraries installed or just Download the required Libraries
NTPclient.h
Base64.h
WiFiUdp.h
esp_http-client.h
esp_camera.h
mbedtls/base64.h
