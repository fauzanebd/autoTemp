#include <SoftwareSerial.h>
#include <Arduino.h>
#include "sendToEsp.h"

String  roomTemp_send = "027.00";
String  counter_send = "100.00";
String  acTemp_send = "026.00";

void sendToESP(int occupancy, float roomTemp, float acTemp,SoftwareSerial NanoSerial){
  if (occupancy < 10){
      counter_send = "00" + String(occupancy) + ".00";
  }
  else {
      counter_send = "0" +String(occupancy) + ".00";
  }
  
  acTemp_send = "0" + String(acTemp) + ".00";
  roomTemp_send = "0" +String(roomTemp) +".00";

 Serial.print(counter_send); Serial.print("\t");
 Serial.print(roomTemp_send); Serial.print("\t");
 Serial.println(acTemp_send);
 
 NanoSerial.print(counter_send); NanoSerial.print("%");
 NanoSerial.print(roomTemp_send); NanoSerial.print("%");
 NanoSerial.print(acTemp_send); NanoSerial.print("\n");
 
 delay(100);
}
