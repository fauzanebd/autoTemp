#ifndef DETECT_TRANSMIT_TEMP_H
#define DETECT_TRANSMIT_TEMP_H

#include <Arduino.h>

int detectTemp(int dht_apin);
void transmitTemp(int temp, int counter);

#endif