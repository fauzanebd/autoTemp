#ifndef DETECT_TRANSMIT_TEMP_H
#define DETECT_TRANSMIT_TEMP_H

#include <Arduino.h>

float detectTemp(int dht_apin);
void transmitTemp(float acTemp, int counter);

#endif