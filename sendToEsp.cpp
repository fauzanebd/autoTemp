String suhu = "027.00";
String orang = "100.00";
String suhuAC = "026.00";

#include <SoftwareSerial.h>

SoftwareSerial NanoSerial(2, 3); // RX | TX

void setup(){
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  
  Serial.begin(9600);
  NanoSerial.begin(57600);
}

void loop(){
//  Serial.print(suhu); Serial.print("\t");
//  Serial.println(orang); Serial.print("\t");
//  Serial.print(suhuAC);
  
  // String dataAll = String(orang) + "," + String(suhuAC);
  // Serial.println(dataAll);

  Serial.print(orang); Serial.print("\t");
  Serial.print(suhu); Serial.print("\t");
  Serial.println(suhuAC);
  
  NanoSerial.print(orang); NanoSerial.print("%");
  NanoSerial.print(suhu); NanoSerial.print("%");
  NanoSerial.print(suhuAC); NanoSerial.print("\n");
  
  delay(100);
}