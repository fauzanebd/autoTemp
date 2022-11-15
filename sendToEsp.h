#ifndef SENDTOESPC_H
#define SENDTOESP_H

#include <Arduino.h>
#include <SoftwareSerial.h>

void sendToESP(int occupancy, float roomtemp, float acTemp,SoftwareSerial NanoSerial);

#endif
