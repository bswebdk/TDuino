/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TPin.cpp  
  
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

#include "TPin.h"
#include <pins_arduino.h>

#ifdef TDUINO_DEBUG
bool TPin::isPinValid(const char *token)
{
  if ((pin != 255) && (mode != 255)) return true;
  TDuino_Error(TDUINO_ERROR_INVALID_PIN, pin, token);
  return false;
}
#endif //TDUINO_DEBUG

void TPin::defaults()
{
  TBase::defaults();
  this->pin = 255;
  this->mode = 255;
  this->analog = false;
}

TPin::TPin() : TBase()
{
  defaults();
}

TPin::TPin(byte pin, byte mode)
{
  defaults();
  attach(pin, mode);
}
  
void TPin::attach(byte pin, byte mode)
{
  this->pin = pin;
  this->mode = mode;
  this->analog = (pin >= A0) && (pin < A0 + NUM_ANALOG_INPUTS);
  pinMode(pin, mode);
}

void TPin::enable(byte on)
{
#ifdef TDUINO_DEBUG
  if (!isPinValid(PSTR("TPin::enable"))) return;
#endif //TDUINO_DEBUG
  digitalWrite(pin, on);
}

void TPin::flip()
{
#ifdef TDUINO_DEBUG
  if (!isPinValid(PSTR("TPin::flip"))) return;
#endif //TDUINO_DEBUG
  digitalWrite(pin, digitalRead(pin) ^ 1);
}

void TPin::off()
{
#ifdef TDUINO_DEBUG
  if (!isPinValid(PSTR("TPin::off"))) return;
#endif //TDUINO_DEBUG
  digitalWrite(pin, LOW);
}

void TPin::on()
{
#ifdef TDUINO_DEBUG
  if (!isPinValid(PSTR("TPin::on"))) return;
#endif //TDUINO_DEBUG
  digitalWrite(pin, HIGH);
}

void TPin::pwm(byte value)
{
#ifdef TDUINO_DEBUG
  const char *token = PSTR("TPin::pwm");
  if (!isPinValid(token)) return;
  if (!hasPwm()) TDuino_Error(TDUINO_ERROR_INVALID_OPERATION, pin, token);
#endif //TDUINO_DEBUG
  analogWrite(pin, value);
}

int TPin::read()
{
#ifdef TDUINO_DEBUG
  if (!isPinValid(PSTR("TPin::read"))) return 0;
#endif //TDUINO_DEBUG
  return analog ? analogRead(pin) : digitalRead(pin);
}

int TPin::state()
{
#ifdef TDUINO_DEBUG
  if (!isPinValid(PSTR("TPin::state"))) return 0;
#endif //TDUINO_DEBUG
   return digitalRead(pin);
}

bool TPin::isAnalog()
{
  return analog;
}

bool TPin::hasPwm()
{
  return digitalPinHasPWM(pin);
}

int TPin::getMode()
{
  return mode;
}

int TPin::getPin()
{
  return pin;
}
