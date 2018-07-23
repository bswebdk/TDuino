/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TDefs.cpp
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>
  
*/


#include "TDefs.h"
//#ifdef TDUINO_DEBUG

byte last_error = 0;
unsigned long lastPrint = 0;

void DebugPrint(bool error, byte number, int *extra, const char *token)
{
  //Flood prevention
  if ((last_error == error) && (millis() - lastPrint < 500)) return;
  lastPrint = millis();
  Serial.print(F("TDuino_"));
  Serial.print(error ? F("Error") : F("Warning"));
  Serial.print(F(" "));
  Serial.print(number, DEC);
  if (extra)
  {
    Serial.print(F(" "));
    if (token) Serial.print((__FlashStringHelper*)token);
    //else Serial.print("none");
    Serial.print(F("("));
    Serial.print(*extra, DEC);
    Serial.print(F(")"));
  }
  Serial.println();
}

void TDuino_Error(byte number, int extra, const char *token) { DebugPrint(true, number, &extra, token); }
void TDuino_Error(byte number, const char *token) { DebugPrint(true, number, NULL, token); }
void TDuino_Error(byte number) { DebugPrint(true, number, NULL, NULL); }

void TDuino_Warning(byte number, int extra, const char *token) { DebugPrint(false, number, &extra, token); }
void TDuino_Warning(byte number, const char *token) { DebugPrint(false, number, NULL, token); }
void TDuino_Warning(byte number) { DebugPrint(false, number, NULL, NULL); }

int freeRam () {
#if defined(__AVR_ATmega328P__) || defined(_AVR_ATmega328__) || defined(_AVR_ATmega168__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
#elif defined(ARDUINO_ESP8266_NODEMCU)
  return node.heap();
#else
  #warning "Could not detect proper board type in TDefs::freeRam()"
  return 1000; //Assumed maximum available memory
#endif
}

//#endif //TDUINO_DEBUG
