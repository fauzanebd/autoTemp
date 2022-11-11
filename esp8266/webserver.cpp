// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid     = "ESP8266_Access_Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .temp-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>AC Automation WebServer</h2>
  <p>
    <span class="temp-labels">ROOM TEMPERATURE</span> 
    <span id="room-temp">%ROOM-TEMP%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="temp-labels">AC TEMPERATURE</span>
    <span id="ac-temp">%AC-TEMP%</span>
    <sup class="units">%&deg/sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("room-temp").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/room-temp", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ac-temp").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/ac-temp", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "ROOM-TEMP"){
    return String(t);
  }
  else if(var == "AC-TEMP"){
    return String(h);
  }
  return String();
}

void setupWebserver(float roomtemp, int actemp){
  // Serial port for debugging purposes
  Serial.begin(115200);
  
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
}
 
//  void updateWebserver(float roomtemp, int actemp) {
//     // Route for root / web page
//   server.update("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send_P(200, "text/html", index_html, processor);
//   });
//   server.update("/room-temp", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send_P(200, "text/plain", String(roomtemp).c_str());
//   });
//   server.update("/ac-temp", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send_P(200, "text/plain", String(actemp).c_str());
//   });
//   server.update()
//  }

