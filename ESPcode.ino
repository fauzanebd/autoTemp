#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

//set up Access point
const char* ssid     = "Capstone A-14 Webserver";
const char* password = "123456789";

//// wificredentials
//const char* ssid = "ououo";
//const char* password = "OmenMaster23";

//variables to get data from nano
SoftwareSerial NodeSerial(D5,D6); // RX | TX

static char arr[3][100];
const unsigned int MAX_MSG = 12;
String org,temp,ac;
int orang, suhu, suhuAC;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <meta http-equiv="X-UA-Compatible" content="ie=edge" />
    <title>Welcome to A-14 Webserver</title>
    <!-- <link rel="stylesheet" href="style.css" /> -->
  </head>
  <style>
    * {
        box-sizing: border-box;
        margin: 0;
        }

        body {
        font-family: "Roboto", sans-serif;
        }

        [class*="col-"] {
        float: left;
        padding: 15px;
        }
        /* For mobile phones: */
        [class*="col-"] {
        width: 100%;
        }

        @media only screen and (min-width: 600px) {
        /* For tablets: */
        .col-s-1 {width: 8.33%;}
        .col-s-2 {width: 16.66%;}
        .col-s-3 {width: 25%;}
        .col-s-4 {width: 33.33%;}
        .col-s-5 {width: 41.66%;}
        .col-s-6 {width: 50%;}
        .col-s-7 {width: 58.33%;}
        .col-s-8 {width: 66.66%;}
        .col-s-9 {width: 75%;}
        .col-s-10 {width: 83.33%;}
        .col-s-11 {width: 91.66%;}
        .col-s-12 {width: 100%;}
        }
        @media only screen and (min-width: 768px) {
        /* For desktop: */
        .col-1 {width: 8.33%;}
        .col-2 {width: 16.66%;}
        .col-3 {width: 25%;}
        .col-4 {width: 33.33%;}
        .col-5 {width: 41.66%;}
        .col-6 {width: 50%;}
        .col-7 {width: 58.33%;}
        .col-8 {width: 66.66%;}
        .col-9 {width: 75%;}
        .col-10 {width: 83.33%;}
        .col-11 {width: 91.66%;}
        .col-12 {width: 100%;}
        }
        /* Navbar */

        .nav-container {
        background: #fffffa;
        box-shadow: 0px 8px 24px rgba(0, 0, 0, 0.1);
        /* height: 80px; */
        margin-top: 0px;
        z-index: 500;
        position: relative;
        }

        .wrapper {
        /* width: 1200px; */
        margin: 0 auto;
        }

        nav {
            display: flex;
            align-items: center;
            justify-content: space-between;
            height: 108px;
        }


        .logo {
        font-family: "Poppins", sans-serif;
        font-style: normal;
        font-weight: 800;
        font-size: 36px;
        color: rgba(66, 142, 255, 1);
        }
        .logo p {
            font-size: 20px;
            font-weight: 600;
            line-height: 0px;
            color: #021718;
            /* width: 418px; */
            margin-top: 24px;
            margin-bottom: 4px;
        }
        span.title-main {
            font-family: 'Poppins', sans-serif;
            font-style: normal;
            font-weight: 800;
            font-size: 40px;
            margin: 0px 8px;
            color: #FF1616;
        }


        /* Page Content */

        .header-container {
        background: rgb(33, 68, 120);
        height: calc(100vh - 108px);
        display: flex;
        align-items: center;
        }

        header {
        display: flex;
        align-items: center;
        justify-content: space-between;
        margin-left: 4%;
        }

        header h1 {
        font-weight: 900;
        font-size: 36px;
        line-height: 75px;
        color: #0f1a92;
        margin-bottom: 4px;
        }

        header p {
        font-size: 20px;
        line-height: 0px;
        color: #021718;
        /* width: 418px; */
        text-align: center;
        margin-bottom: 24px;
        }



        /* card */


        .row-card {
        display: flex;
        align-items: center;
        align-content: center;
        gap: 24px;
        width: 90vw;
        content: "";
        clear: both;
        
        }

        .row-card .card{
            box-sizing: border-box;

            display: flex;
            flex-direction: column;
            align-items: center;
            padding: 0px 40px;

            width: 34vw;
            height: 50%;

            background: #FFF4F4;
            border: 1px solid #000000;
            box-shadow: 0px 4px 4px rgba(0, 0, 0, 0.25);
            border-radius: 25px;

        }


        .units-container{
            display: flex;
            flex-direction: row;
            justify-content: center;
            align-items: flex-start;
            padding: 0px 0px 0px 80px;
            gap: 10px;
            margin-top: -32px;
            /* width: 362px;
            height: 180px; */
            margin-left: 20px;
        }

        .units-container .temp{
            /* width: 200px;
            height: 180px; */

            font-family: "Poppins", sans-serif;
            font-style: normal;
            font-weight: 800;
            font-size: 144px;
            line-height: 216px;
            display: flex;
            align-items: center;
            text-align: center;

            color: #2175F4;
        }

        .units{
            /* width: 72px;
            height: 83px; */

            font-family: "Poppins", sans-serif;
            font-style: normal;
            font-weight: 600;
            font-size: 72px;
            line-height: 108px;
            display: flex;
            align-items: center;
            text-align: center;
            margin-left: 8px;
            margin-top: 32px;

            color: #81B3FF;

        }

        .column-card .card-occupant{
            box-sizing: border-box;

            display: flex;
            flex-direction: column;
            align-items: center;
            margin-left: 0px;
            margin-top: 24px;

            width: 70vw;
            height: 50%;

            background: #FFF4F4;
            border: 1px solid #000000;
            box-shadow: 0px 4px 4px rgba(0, 0, 0, 0.25);
            border-radius: 25px;

        }

        .occupant{
            /* width: 200px;
            height: 180px; */

            font-family: "Poppins", sans-serif;
            font-style: normal;
            font-weight: 800;
            font-size: 144px;
            line-height: 216px;
            display: flex;
            align-items: center;
            text-align: center;
            justify-content: center;

            color: #2175F4;

        }

        .units-occupant{
            /* width: 72px;
            height: 83px; */

            font-family: "Poppins", sans-serif;
            font-style: normal;
            font-weight: 600;
            font-size: 36px;
            line-height: 108px;
            display: flex;
            align-items: center;
            text-align: center;
            margin-left: 0px;
            margin-top: 12px;

            color: #81B3FF;
        }
  </style>
  <body>

    <div class="nav-container">
      <div class="wrapper">
        <nav>
          <div class="logo">
            <p>AC Automation Webserver</p>
            <p>What is<span class="title-main">Currently Happening</span>in This Room?</p>
          </div>
        </nav>
      </div>
    </div>

    <div class="header-container">
      <div class="wrapper">
        <header>
            <div class="column-card">
            <div class="row-card">
                <div class="card">
                    <h1>Room Temperature</h1>
                    <p>at the moment</p>
                    <div class="units-container">
                        <span class="temp" id="room-temp">%ROOM-TEMP%</span>
                        <sup class="units">&deg;C</sup>
                    </div>
                </div>
                <div class="card">
                    <h1>AC Temperature</h1>
                    <p>at the moment</p>
                    <div class="units-container">
                        <span class="temp" id="ac-temp">%AC-TEMP%</span>
                        <sup class="units">&deg;C</sup>
                    </div>
                </div>
            </div>
            <div class="card-occupant">
                <h1>Room Occupancy</h1>
                <p>at the moment</p>
                <div class="units-container">
                    <span class="occupant" id="occupancy">%OCCUPANCY%</span>
                    <sup class="units-occupant">people</sup>
                </div>
            </div>
        </div>

        </header>


      </div>
    </div>

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
    setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
        document.getElementById("occupancy").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "/occupancy", true);
    xhttp.send();
    }, 10000 ) ;
    
    setTimeout("location.reload(true);", 3000);

  </script>
</html>
)rawliteral";


// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "ROOM-TEMP"){
    return String(suhu);
  }
  else if(var == "AC-TEMP"){
    return String(suhuAC);
  }
  else if(var == "OCCUPANCY"){
    return String(orang);
  }
  return String();
}


void setup(){
  pinMode(D5, INPUT);
  pinMode(D6, OUTPUT);
  
  Serial.begin(9600);
  NodeSerial.begin(57600);

  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());


//  // Connect to Wi-Fi
//  WiFi.begin(ssid, password);
//  Serial.println("Connecting to WiFi");
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(1000);
//    Serial.println(".");
//  }
//
//  // Print ESP8266 Local IP Address
//  Serial.println(WiFi.localIP());
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/room-temp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(suhu).c_str());
  });
  server.on("/ac-temp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(suhuAC).c_str());
  });
  server.on("/occupancy", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(orang).c_str());
  });

  // Start server
  server.begin();
 
}


void loop(){
  char dataRec[MAX_MSG] = "" ;
  unsigned int message_pos = 0;
  
  while (NodeSerial.available() > 0){
    char byteRead = NodeSerial.read();
    if (byteRead != '\n' && (message_pos < MAX_MSG-1)){
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
    int arr_pos = 0;
    
    arr[0][0] = '\0';
    arr[1][0] = '\0';
    arr[2][0] = '\0';
    
    Serial.println(message_pos);
    for (int i = 0; i< message_pos; i++){
//      Serial.println(dataRec[i]);
      char delim = ',';
      
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
        org = String(arr[0]);
        orang = org.toInt();
//        orang = twoDigits(org, orang);
        Serial.println(org);
        Serial.println(orang);
        Serial.print("Suhu: "); Serial.print(arr[1]); Serial.println("");
        temp = String(arr[1]);
        suhu = temp.toInt();
//        suhu = twoDigits(temp, suhu);
        Serial.println(temp);
        Serial.println(suhu);
        Serial.print("AC: "); Serial.print(arr[2]); Serial.println("");
        ac = String(arr[2]);
        suhuAC = ac.toInt();
//        suhuAC = twoDigits(ac, suhuAC);
        Serial.println(ac);
        Serial.println(suhuAC);
      }
//          
    }
    for (int x = 0; x < 100; x++)
    {
      arr[0][x]='\0';
      arr[1][x]='\0';
      arr[2][x]='\0';
    }
//    arr[0][0] = '\0';
//    arr[1][0] = '\0';
//    arr[2][0] = '\0';
  }
  delay(500);


}

int twoDigits(String temp, int suhu){
  int new_suhu;
  if (suhu > 100){
    int x = temp.length() - 3;
    new_suhu = floor(suhu/(pow(10,x)));
  }
  return new_suhu;  
}