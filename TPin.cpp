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

bool TPin::isPwmValid(int &value, const char *token)
{
  if (value >= 0) return true;
  TDuino_Error(TDUINO_ERROR_BAD_PARAMETER, value, token);
  return false;
}

#endif //TDUINO_DEBUG

void TPin::defaults()
{
  TBase::defaults();
  this->pin = 255;
  this->mode = 255;
}

TPin::TPin() : TBase()
{
  defaults();
}

void TPin::attach(byte pin, byte mode)
{
  pinMode(pin, mode);
  this->pin = pin;
  this->mode = mode;
  if ((pin >= A0) && (pin < A0 + NUM_ANALOG_INPUTS)) this->mode |= ANALOG_BIT;
  if (digitalPinHasPWM(pin)) this->mode |= PWM_BIT;
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

void TPin::pwm(int value)
{
#ifdef TDUINO_DEBUG
  const char *token = PSTR("TPin::pwm");
  if (!isPinValid(token)) return;
  if (!hasPwm()) { TDuino_Error(TDUINO_ERROR_INVALID_OPERATION, pin, token); return; }
  if (!isPwmValid(value, token)) return;
#endif //TDUINO_DEBUG
  analogWrite(pin, value);
}

int TPin::read()
{
#ifdef TDUINO_DEBUG
  if (!isPinValid(PSTR("TPin::read"))) return 0;
#endif //TDUINO_DEBUG
  return (mode & ANALOG_BIT) ? analogRead(pin) : digitalRead(pin);
}

byte TPin::state()
{
#ifdef TDUINO_DEBUG
  if (!isPinValid(PSTR("TPin::state"))) return 0;
#endif //TDUINO_DEBUG
   return digitalRead(pin);
}

bool TPin::isAnalog()
{
  return (mode & ANALOG_BIT);
}

bool TPin::hasPwm()
{
  return (mode & PWM_BIT);//digitalPinHasPWM(pin);
}

byte TPin::getMode()
{
  return mode & 0x0F;
}

byte TPin::getPin()
{
  return pin;
}
