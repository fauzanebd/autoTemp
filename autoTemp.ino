#include "fuzzy_logic.h"
#include "detectTransmitTemp.h"
#include "peopleCounter.h"
#include "sendToEsp.h"

#include <SoftwareSerial.h>

//DHT related pin
#define dht_apin A0 // Analog Pin sensor is connected to

//laser related pin
int in_laser = 10;
int out_laser = 11;

//IR related pin

//ESP Connn related pin
int rx_esp = 2;
int tx_esp = 3;
SoftwareSerial NanoSerial(rx_esp, tx_esp); // RX | TX


//other variables
float roomTemp = 0;
float acTemp = 0;


void setup() {
    //laser setup
    pinMode(in_laser, INPUT);
    pinMode(out_laser, INPUT);

    //ESP Conn setup
    pinMode(rx_esp,INPUT);
    pinMode(tx_esp,OUTPUT);
    NanoSerial.begin(57600);


    Serial.begin(9600);
}

void loop() {
    roomTemp = detectTemp();
    int occupancy = loopCounter(in_laser, out_laser);
    acTemp = getTemperature(roomTemp, occupancy);
    transmitTemp(acTemp, occupancy);
    delay(1000);
}
