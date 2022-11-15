#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>

void setupWebserver(String roomtemp, String actemp);
void updateWebserver(String roomtemp, String actemp);
String processor(const String& var);

const char index_html[] PROGMEM ;

#endif
