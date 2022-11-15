#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webserver.h"

SoftwareSerial NodeSerial(D5,D6); // RX | TX
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

static char arr[3][100];
const unsigned int MAX_MSG = 21;
static char actemp;
static char roomtemp;
static char people;
const char* ssid     = "ESP8266_Access_Point";
const char* password = "123456789";

void setup(){
  pinMode(D5, INPUT);
  pinMode(D6, OUTPUT);
  
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/room-temp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(roomtemp).c_str());
  });
  server.on("/ac-temp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(actemp).c_str());
  });

  // Start server
  server.begin();
  
  Serial.begin(9600);
  NodeSerial.begin(57600);
}

void loop(){
  static char dataRec[MAX_MSG] = "" ;
  static unsigned int message_pos = 0;
  
  while (NodeSerial.available() > 0){
    char byteRead = NodeSerial.read();
    if (byteRead != '\n' && (message_pos < MAX_MSG -1)){
      dataRec[message_pos] = byteRead;
      message_pos++;  
    }
    else {
//      dataRec[message_pos] = '\0';
      Serial.println(dataRec);
      break;
      
      
      // reset
//      message_pos = 0;
    }
  }
  if (dataRec != ""){
    
    int index = 0;
    arr[0][0] = '\0';
    arr[1][0] = '\0';
    arr[2][0] = '\0';
    int arr_pos = 0;
    Serial.println(message_pos);
    for (int i = 0; i< message_pos; i++){
      
//      Serial.println(dataRec[i]);
      char delim = '%';
      if (dataRec[i] != delim){
        arr[index][arr_pos] = dataRec[i];
        arr_pos++;
//        Serial.println(dataRec[i]);
      }
      else {
        arr_pos = 0;
//            Serial.print("nyimpen: "); Serial.println(arr[index]);
        if (index < 2){
          index++;
        } else {
          index = 0;              
        }
      }
//          
      if (index == 2){
        Serial.print("Orang: "); Serial.print(arr[0]); Serial.println("");
        Serial.print("Suhu: "); Serial.print(arr[1]); Serial.println("");
        Serial.print("AC: "); Serial.print(arr[2]); Serial.println("");
        actemp = arr[2];
        roomtemp = arr[1];
        people = arr[0];
      }
//          
    }
  }
  delay(500);

}
