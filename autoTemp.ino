#include "fuzzy_logic.h"
#include "detectTransmitTemp.h"
#include "webserver.h"
#include "peopleCounter.h"


int in_laser = 10;
int out_laser = 11;
float roomtemp = 0;
int actemp = 0;


void setup() {
    Serial.begin(9600);

    setupCounter(in_laser, out_laser)
}

void loop() {
    roomTemp = detectTemp();
    int occupancy = loopCounter(in_laser, out_laser);

    actemp = getTemperature(roomTemp, 10));
    delay(1000);
}
