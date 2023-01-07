#include "fuzzy_logic.h"
#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender
#include <dht.h>
#include <Arduino.h>

IRsendRaw mySender;

#include <SoftwareSerial.h>

//DHT related pin
#define dht_apin 4 // Analog Pin sensor is connected to    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define RAW_DATA_LEN 140
#define RAW_DATA_LIST_LEN 17

uint16_t sendDataTemp[100];

dht DHT;

//laser related pin
int in_laser = 7;
int out_laser = 8;

//IR related pin

//Nano Connn related pin
int rx_nano = 10;
int tx_nano = 11;

SoftwareSerial NanoSerial(rx_nano, tx_nano); // RX | TX

#define RAW_DATA_LEN 100

//off
const uint16_t rawDataOff[RAW_DATA_LEN] PROGMEM={
  4382, 4466, 482, 1678, 458, 614, 482, 1654, 
  478, 1674, 458, 618, 478, 614, 482, 1654, 
  478, 614, 478, 618, 478, 1674, 458, 618, 
  474, 618, 478, 1678, 458, 1678, 458, 614, 
  478, 1658, 478, 614, 478, 1678, 458, 1658, 
  482, 1674, 458, 1674, 462, 614, 478, 1658, 
  478, 1674, 462, 1654, 482, 614, 478, 614, 
  482, 610, 482, 614, 482, 1654, 478, 614, 
  482, 614, 478, 1658, 478, 1678, 458, 1654, 
  482, 614, 482, 610, 482, 614, 478, 618, 
  478, 614, 478, 614, 482, 614, 482, 610, 
  482, 1654, 482, 1654, 482, 1674, 462, 1674, 
  490, 1626, 506, 1000};

//on
const uint16_t rawDataOn[RAW_DATA_LEN] PROGMEM={
  4386, 4466, 482, 1674, 486, 586, 514, 1646, 
  482, 1630, 482, 610, 482, 610, 514, 1622, 
  510, 586, 478, 614, 510, 1626, 506, 586, 
  510, 586, 482, 1654, 506, 1630, 506, 586, 
  506, 1630, 510, 582, 510, 586, 482, 1654, 
  506, 1626, 482, 1654, 510, 1626, 510, 1626, 
  506, 1630, 506, 1630, 506, 1630, 506, 586, 
  506, 590, 510, 582, 510, 586, 506, 586, 
  510, 582, 510, 586, 514, 578, 514, 582, 
  510, 1626, 510, 586, 510, 582, 514, 582, 
  510, 582, 514, 1622, 514, 1622, 514, 1622, 
  490, 606, 486, 1650, 490, 1646, 514, 1622, 
  518, 1618, 522, 1000};

const uint16_t temp16[RAW_DATA_LEN] PROGMEM={
   4342, 4494, 478, 1678, 462, 614, 482, 1654, 
   478, 1678, 458, 614, 478, 618, 482, 1674, 
   458, 614, 482, 614, 478, 1678, 458, 614, 
   482, 614, 506, 1650, 458, 1678, 458, 614, 
   482, 1674, 490, 1626, 506, 590, 482, 1654, 
   482, 1674, 462, 1654, 482, 1674, 462, 1674, 
   462, 1674, 462, 614, 482, 1674, 458, 618, 
   478, 614, 482, 614, 478, 614, 482, 614, 
   482, 614, 478, 614, 482, 614, 482, 610, 
   482, 614, 482, 614, 482, 610, 482, 614, 
   486, 610, 482, 1674, 486, 1650, 462, 1674, 
   462, 1654, 482, 1674, 462, 1678, 462, 1674, 
   462, 1674, 462, 1000};

const uint16_t temp17[RAW_DATA_LEN] PROGMEM={
   4386, 4474, 474, 1658, 478, 650, 442, 1662, 
   474, 1662, 474, 650, 442, 650, 446, 1658, 
   478, 650, 442, 650, 446, 1662, 474, 650, 
   446, 646, 446, 1662, 474, 1658, 478, 650, 
   442, 1662, 478, 1658, 478, 646, 446, 1662, 
   474, 1662, 474, 1662, 474, 1662, 474, 1662, 
   474, 1662, 474, 650, 446, 1662, 474, 646, 
   450, 646, 446, 646, 450, 646, 450, 646, 
   446, 646, 450, 646, 450, 642, 450, 646, 
   450, 646, 450, 642, 450, 646, 450, 642, 
   454, 642, 450, 1658, 478, 1658, 482, 1658, 
   478, 1658, 478, 1658, 478, 1662, 478, 1654, 
   482, 1654, 482, 1000};

const uint16_t temp18[RAW_DATA_LEN] PROGMEM={
   4386, 4470, 474, 1662, 474, 646, 446, 1662, 
   474, 1662, 474, 646, 450, 642, 450, 1662, 
   474, 646, 450, 642, 454, 1658, 478, 642, 
   450, 642, 454, 1658, 478, 1658, 478, 642, 
   450, 1662, 474, 1662, 474, 646, 450, 1662, 
   474, 1658, 478, 1658, 478, 1662, 474, 1662, 
   474, 1662, 474, 646, 450, 1658, 478, 642, 
   450, 646, 450, 642, 454, 642, 450, 646, 
   450, 642, 454, 642, 450, 646, 450, 642, 
   450, 1662, 478, 642, 450, 642, 454, 642, 
   454, 642, 450, 1662, 478, 1658, 478, 1658, 
   478, 642, 454, 1658, 478, 1658, 478, 1658, 
   478, 1658, 478, 1000};

const uint16_t temp19[RAW_DATA_LEN] PROGMEM={
   4386, 4474, 474, 1658, 478, 650, 442, 1662, 
   474, 1662, 474, 650, 442, 650, 446, 1658, 
   478, 650, 442, 650, 446, 1662, 474, 650, 
   446, 646, 446, 1662, 474, 1658, 478, 650, 
   442, 1662, 478, 1658, 478, 646, 446, 1662, 
   474, 1662, 474, 1662, 474, 1662, 474, 1662, 
   474, 1662, 474, 650, 446, 1662, 474, 646, 
   450, 646, 446, 646, 450, 646, 450, 646, 
   446, 646, 450, 646, 450, 642, 450, 646, 
   450, 646, 450, 642, 450, 646, 450, 642, 
   454, 642, 450, 1658, 478, 1658, 482, 1658, 
   478, 1658, 478, 1658, 478, 1662, 478, 1654, 
   482, 1654, 482, 1000};

const uint16_t temp20[RAW_DATA_LEN] PROGMEM={
   4386, 4474, 474, 1658, 478, 650, 442, 1662, 
   474, 1662, 474, 650, 442, 650, 446, 1658, 
   478, 650, 442, 650, 446, 1662, 474, 650, 
   446, 646, 446, 1662, 474, 1658, 478, 650, 
   442, 1662, 478, 1658, 478, 646, 446, 1662, 
   474, 1662, 474, 1662, 474, 1662, 474, 1662, 
   474, 1662, 474, 650, 446, 1662, 474, 646, 
   450, 646, 446, 646, 450, 646, 450, 646, 
   446, 646, 450, 646, 450, 642, 450, 646, 
   450, 646, 450, 642, 450, 646, 450, 642, 
   454, 642, 450, 1658, 478, 1658, 482, 1658, 
   478, 1658, 478, 1658, 478, 1662, 478, 1654, 
   482, 1654, 482, 1000};

const uint16_t temp21[RAW_DATA_LEN] PROGMEM={
   4366, 4490, 458, 1678, 458, 618, 478, 1678, 
   458, 1678, 458, 614, 482, 614, 478, 1678, 
   458, 618, 478, 614, 478, 1678, 458, 618, 
   478, 614, 482, 1678, 458, 1674, 462, 614, 
   482, 1674, 462, 1674, 462, 614, 478, 1678, 
   458, 1658, 478, 1678, 458, 1678, 462, 1674, 
   462, 1674, 462, 614, 478, 1678, 458, 618, 
   478, 614, 482, 614, 478, 618, 478, 614, 
   478, 618, 478, 618, 478, 1678, 458, 1682, 
   454, 618, 478, 614, 482, 614, 478, 618, 
   478, 614, 478, 1662, 478, 614, 482, 614, 
   478, 1658, 482, 1674, 462, 1678, 458, 1678, 
   458, 1678, 458, 1000};

const uint16_t temp22[RAW_DATA_LEN] PROGMEM={
   4394, 4466, 514, 1618, 518, 578, 514, 1622, 
   514, 1622, 514, 578, 518, 578, 514, 1622, 
   514, 578, 518, 578, 514, 1626, 510, 582, 
   514, 578, 518, 1618, 518, 1622, 514, 578, 
   514, 1622, 514, 1626, 510, 582, 514, 1622, 
   514, 1622, 514, 1622, 514, 1622, 514, 1626, 
   514, 1622, 514, 578, 514, 1626, 514, 578, 
   514, 582, 514, 578, 514, 582, 514, 582, 
   514, 578, 514, 582, 514, 1622, 514, 1626, 
   510, 1622, 514, 582, 514, 578, 518, 578, 
   518, 578, 514, 1622, 518, 578, 514, 582, 
   514, 578, 518, 1622, 514, 1622, 514, 1622, 
   514, 1626, 510, 1000};

const uint16_t temp23[RAW_DATA_LEN] PROGMEM={
   4366, 4490, 462, 1674, 462, 614, 478, 1674, 
   494, 1646, 490, 582, 482, 614, 478, 1678, 
   490, 586, 506, 586, 510, 1646, 462, 614, 
   510, 582, 510, 1646, 466, 1670, 498, 578, 
   510, 1646, 462, 1674, 490, 586, 482, 1674, 
   490, 1646, 462, 1674, 462, 1674, 466, 1670, 
   490, 1646, 490, 586, 510, 1646, 462, 614, 
   514, 582, 510, 586, 506, 586, 510, 586, 
   482, 614, 478, 626, 498, 1646, 494, 582, 
   510, 1642, 498, 582, 510, 582, 510, 586, 
   510, 586, 506, 1646, 494, 586, 514, 1638, 
   490, 590, 506, 1646, 494, 1646, 462, 1658, 
   506, 1630, 510, 1000};

const uint16_t temp24[RAW_DATA_LEN] PROGMEM={
   4370, 4486, 490, 1646, 486, 610, 490, 1642, 
   490, 1646, 466, 634, 490, 606, 458, 1670, 
   494, 606, 486, 586, 510, 1642, 466, 634, 
   458, 638, 482, 1650, 490, 1646, 494, 606, 
   458, 1670, 466, 1674, 494, 602, 490, 1642, 
   494, 1642, 498, 1638, 494, 1642, 462, 1674, 
   494, 1646, 490, 586, 506, 1646, 494, 586, 
   506, 586, 510, 586, 482, 614, 510, 582, 
   506, 590, 506, 610, 462, 1670, 490, 586, 
   510, 606, 490, 606, 486, 610, 458, 614, 
   482, 610, 514, 1642, 494, 602, 494, 1642, 
   466, 1674, 490, 1646, 462, 1674, 466, 1670, 
   490, 1650, 490, 1000};

const uint16_t temp25[RAW_DATA_LEN] PROGMEM={
   4398, 4462, 514, 1622, 514, 578, 518, 1622, 
   514, 1618, 518, 578, 514, 578, 518, 1622, 
   514, 578, 514, 582, 514, 1622, 514, 578, 
   518, 578, 514, 1622, 514, 1622, 514, 582, 
   514, 1622, 514, 1622, 514, 582, 514, 1622, 
   514, 1622, 514, 1622, 514, 1622, 514, 1622, 
   514, 1622, 514, 582, 514, 1622, 514, 582, 
   514, 578, 514, 582, 514, 582, 514, 578, 
   518, 578, 514, 1622, 514, 1626, 514, 578, 
   514, 582, 514, 578, 518, 578, 514, 582, 
   514, 578, 518, 578, 514, 582, 514, 1622, 
   514, 1622, 514, 1626, 514, 1622, 514, 1622, 
   514, 1622, 514, 1000};

const uint16_t temp26[RAW_DATA_LEN] PROGMEM={
  4394, 4462, 514, 1622, 514, 582, 514, 1622, 
  514, 1622, 514, 578, 518, 578, 514, 1622, 
  514, 582, 514, 578, 514, 1626, 510, 582, 
  514, 578, 518, 1622, 514, 1618, 518, 578, 
  514, 1622, 514, 1626, 510, 582, 514, 1622, 
  514, 1622, 514, 1626, 510, 1626, 514, 1622, 
  514, 1622, 514, 578, 514, 1626, 510, 582, 
  514, 582, 514, 578, 514, 582, 514, 582, 
  514, 578, 514, 1622, 518, 1622, 514, 578, 
  514, 1626, 514, 578, 514, 582, 514, 578, 
  518, 578, 514, 582, 514, 578, 518, 1622, 
  514, 582, 510, 1626, 514, 1622, 514, 1622, 
  514, 1626, 514, 1000};

const uint16_t temp27[RAW_DATA_LEN] PROGMEM={
  4366, 4490, 514, 1622, 514, 582, 514, 1622, 
  514, 1622, 514, 582, 510, 578, 518, 1622, 
  514, 578, 514, 582, 514, 1622, 514, 582, 
  514, 578, 514, 1622, 514, 1622, 518, 578, 
  514, 1622, 514, 1622, 514, 578, 518, 1622, 
  514, 1622, 514, 1622, 514, 1622, 514, 1622, 
  514, 1626, 510, 582, 514, 1622, 514, 582, 
  514, 582, 490, 602, 490, 606, 494, 602, 
  486, 606, 490, 1646, 490, 606, 490, 606, 
  486, 1650, 490, 602, 494, 602, 490, 606, 
  490, 602, 490, 606, 490, 1650, 486, 1650, 
  486, 606, 490, 1650, 490, 1646, 490, 1646, 
  490, 1646, 494, 1000};

const uint16_t temp28[RAW_DATA_LEN] PROGMEM={
  4394, 4462, 514, 1622, 514, 582, 514, 1622, 
  514, 1622, 514, 578, 514, 582, 514, 1622, 
  514, 578, 518, 578, 514, 1622, 514, 578, 
  518, 578, 514, 1622, 514, 1626, 510, 582, 
  514, 1622, 514, 1626, 510, 582, 514, 1622, 
  514, 1622, 514, 1622, 514, 1622, 514, 1622, 
  514, 1622, 518, 578, 514, 1622, 514, 582, 
  514, 578, 518, 578, 514, 582, 514, 578, 
  518, 578, 514, 1622, 514, 582, 514, 578, 
  518, 578, 514, 582, 514, 578, 518, 578, 
  514, 582, 514, 578, 518, 1622, 514, 1622, 
  514, 1622, 514, 1622, 514, 1626, 510, 1626, 
  514, 1626, 510, 1000};

const uint16_t temp29[RAW_DATA_LEN] PROGMEM={
  4390, 4470, 510, 1646, 486, 586, 510, 1626, 
  510, 1626, 510, 586, 482, 610, 510, 1630, 
  506, 586, 482, 614, 510, 1626, 506, 586, 
  482, 614, 482, 1654, 482, 1654, 506, 590, 
  506, 1650, 486, 1650, 490, 586, 506, 1630, 
  510, 1626, 510, 1650, 458, 1658, 478, 1658, 
  478, 1678, 462, 610, 486, 1654, 506, 586, 
  510, 586, 510, 586, 482, 610, 486, 610, 
  506, 590, 506, 1630, 482, 614, 506, 1630, 
  482, 610, 510, 586, 506, 590, 510, 586, 
  506, 586, 510, 586, 510, 1626, 514, 582, 
  510, 1630, 510, 1626, 514, 1622, 514, 1626, 
  514, 1622, 514, 1000};

const uint16_t temp30[RAW_DATA_LEN] PROGMEM={
  4370, 4486, 514, 1626, 510, 582, 514, 1622, 
  514, 1622, 514, 578, 514, 582, 514, 1622, 
  514, 582, 510, 582, 514, 1622, 514, 582, 
  514, 578, 514, 1622, 514, 1622, 514, 582, 
  514, 1622, 514, 1622, 514, 582, 514, 1622, 
  514, 1622, 514, 1622, 514, 1622, 514, 1622, 
  514, 1626, 486, 606, 490, 1646, 514, 582, 
  490, 606, 486, 606, 490, 606, 490, 602, 
  490, 606, 490, 1646, 490, 606, 490, 1646, 
  490, 1650, 490, 602, 490, 606, 490, 606, 
  490, 602, 490, 606, 490, 1646, 494, 602, 
  490, 602, 494, 1646, 494, 1642, 518, 1618, 
  522, 1614, 522, 1000};

   
//other variables
int roomTemp = 0;
int acTemp = 0;
int lastTransmittedACTemp = 0;
int ppl =0;
int delayAmount = 10000;
int idleTime = 10000;
bool acOn = false;
bool turnedOffbyIdle = false;
unsigned long timeSinceLastPeopleIn = 0;
unsigned long timeSinceFirstSensorPassedBy = 0;

String counter_send = "";
String acTemp_send = "";
String roomTemp_send = "";

void setup() {
    //laser setup
    pinMode(in_laser, INPUT);
    pinMode(out_laser, INPUT);

    //Nano Conn setup
    pinMode(rx_nano,INPUT);
    pinMode(tx_nano,OUTPUT);
    NanoSerial.begin(57600);

    Serial.begin(9600);
}

void loop() {
    
//    int occupancy = loopCounter(in_laser, out_laser);
    // hitung org
    while (true){
      
     bool pi = digitalRead(in_laser);
     bool po = digitalRead(out_laser);
     if (pi == 0) {
        Serial.println("Reading people in");
        timeSinceFirstSensorPassedBy = millis();
       while (true) {
         po = digitalRead(out_laser);
         if(millis() - timeSinceFirstSensorPassedBy > 2000){break;}
         if (po == 0) {
           ppl++;
            Serial.println("===Temperature changed because 1 people in===");  
            timeSinceLastPeopleIn = millis();
            turnedOffbyIdle = false;
            roomTemp = detectTemp();
            Serial.print("Current room temp:"); Serial.println(roomTemp);
            Serial.print("Current occupancy: "); Serial.println(ppl);
            acTemp = getTemperature(roomTemp, ppl);
            Serial.print("AC temp: ");
            Serial.println(acTemp);
            transmitTemp(acTemp, ppl);
            sendToESP(ppl, int(roomTemp), int(acTemp));
           delay(100);
           break;
         }
       }
       break;
     }
     else if (po == 0) {
        Serial.println("Reading people out");
        timeSinceFirstSensorPassedBy = millis();
       while (true) {
         pi = digitalRead(in_laser);
         if(millis() - timeSinceFirstSensorPassedBy > 2000){break;}
         if (pi == 0) {
           if (ppl > 0) {
              ppl--; 
           }
            Serial.println("===Temperature changed because 1 people out===");            
            timeSinceLastPeopleIn = millis();
            turnedOffbyIdle = false;
            roomTemp = detectTemp();
            Serial.print("Current room temp:"); Serial.println(roomTemp);
            Serial.print("Current occupancy: "); Serial.println(ppl);            
            acTemp = getTemperature(roomTemp, ppl);
            Serial.print("AC temp :");
            Serial.println(acTemp);
            transmitTemp(acTemp, ppl);
            sendToESP(ppl, int(roomTemp), int(acTemp));
           delay(100);
           break;
         }
       }
       break;
     }
     
     if (millis() - timeSinceLastPeopleIn >= idleTime) {
        roomTemp = detectTemp();
        Serial.println("=== System detected idle condition ===");
        Serial.print("Current room temp: "); Serial.println(roomTemp);
        Serial.print("Current Occupancy: "); Serial.println(ppl);
        sendToESP(ppl, int(roomTemp), int(acTemp));
        if (ppl == 0 && turnedOffbyIdle == 0){
          transmitTemp(0, 0);
          sendToESP(ppl, int(roomTemp), int(acTemp));
          turnedOffbyIdle = true;
          break;
        } else if (ppl > 0) {
          
          acTemp = getTemperature(roomTemp, ppl);
          if (lastTransmittedACTemp == acTemp){break;}
          Serial.print("AC Temp :");
          Serial.println(acTemp);
          transmitTemp(acTemp, ppl);
          sendToESP(ppl, int(roomTemp), int(acTemp));
          break;
        } else {
          Serial.println("Nothing happened");
          break;
        }
      }
    }
//    delay(delayAmount);
    
}

//use DHT sensor to detect room temp
int detectTemp() {
  DHT.read22(dht_apin);
  int temp = DHT.temperature;
  return temp;
};

//use IR sensor to transmit command to AC unit
void transmitTemp(int acTemp, int counter){
   lastTransmittedACTemp = acTemp;
   if (counter == 0){
       for(byte i=0; i<100; i++){
          sendDataTemp[i] = pgm_read_word_near(rawDataOff + i);
       };
       mySender.send(sendDataTemp,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
       acOn = false;
       Serial.println("turning AC off");
   }
   else {
    if (acOn) {
       Serial.println("Ac already on");
       setTemp(acTemp);       
    } else {
       for(byte i=0; i<100; i++){
          sendDataTemp[i] = pgm_read_word_near(rawDataOn + i);
       };
       mySender.send(sendDataTemp,RAW_DATA_LEN,36);
       Serial.println("turning AC on");
       acOn = true;
       setTemp(acTemp);
    }
   }
};

void setTemp(int acTemp) {
  if (acTemp == 16){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp16 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
  else if (acTemp == 17){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp17 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
  else if (acTemp == 18){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp18 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
  else if (acTemp == 19){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp19 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
  else if (acTemp == 20){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp20 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
  else if (acTemp == 21){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp21 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
  else if (acTemp == 22){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp22 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
  else if (acTemp == 23){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp23 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
  else if (acTemp == 24){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp24 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
  else if (acTemp == 25){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp25 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
    else if (acTemp == 26){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp26 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
    else if (acTemp == 27){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp27 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
    else if (acTemp == 28){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp28 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
    else if (acTemp == 29){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp29 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
    else if (acTemp == 30){
     for(byte i=0; i<100; i++){
        sendDataTemp[i] = pgm_read_word_near(temp30 + i);
     };
     mySender.send(sendDataTemp,RAW_DATA_LEN,36);
  }
  Serial.print("AC temp set to: "); Serial.println(acTemp);
}

void sendToESP(int occupancy, int roomTemp, int acTemp){
  counter_send = String(occupancy);
  acTemp_send = String(acTemp);
  roomTemp_send = String(roomTemp);
  if (occupancy < 10){
      counter_send = "00" + String(occupancy);
  }
  else {
      counter_send = "0" +String(occupancy);
  }

  
  if (acTemp < 10){
      acTemp_send = "00" + String(acTemp);
  }
  else {
      acTemp_send = "0" +String(acTemp);
  }

  if (roomTemp < 10){
      roomTemp_send = "00" + String(roomTemp);
  }
  else {
      roomTemp_send = "0" +String(roomTemp);
  }

 Serial.print(counter_send); Serial.print("\t");
 Serial.print(roomTemp_send); Serial.print("\t");
 Serial.println(acTemp_send);
 
 NanoSerial.print(counter_send); NanoSerial.print(",");
 NanoSerial.print(roomTemp_send); NanoSerial.print(",");
 NanoSerial.print(acTemp_send); NanoSerial.print("\n");
}