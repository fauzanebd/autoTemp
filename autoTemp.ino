#include "fuzzy_logic.h"


void setup() {
    Serial.begin(9600);
}

void loop() {
    float roomTemp = 25;
    int occupancy = 10;
    Serial.println(getTemperature(roomTemp, 10));
    delay(1000);
}
