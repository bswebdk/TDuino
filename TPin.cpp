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

TPin::TPin(byte pin, byte mode) : TBase()
{
  this->pin = pin;
  setMode(mode);
}
  
void TPin::enable(byte on)
{
  digitalWrite(pin, on);
}

void TPin::flip()
{
  digitalWrite(pin, digitalRead(pin) ^ 1);
}

void TPin::off()
{
  digitalWrite(pin, LOW);
}

void TPin::on()
{
  digitalWrite(pin, HIGH);
}

void TPin::pwm(byte value)
{
#ifdef TDUINO_DEBUG
  if (!hasPwm()) TDuino_Error(TDUINO_ERROR_INVALID_OPERATION, pin, PSTR("TPin::pwm"));
#endif
  analogWrite(pin, value);
}

int TPin::read()
{
  return analog ? analogRead(pin) : digitalRead(pin);
}

int TPin::state()
{
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

void TPin::setMode(int mode)
{
  this->mode = mode;
  this->analog = (pin >= A0) && (pin < A0 + NUM_ANALOG_INPUTS);
  pinMode(pin, mode);
}


int TPin::getPin()
{
  return pin;
}

void TPin::setup()
{
  //Following line is not needed
  //pinMode(pin, mode);
}

//TPin code : 5364 bytes / 17%  || 3788 bytes / 12%
//TPin vars: 289  bytes / 14%  || 272  bytes / 13%
//TPinExt code : 5474 bytes / 17%
//TPinExt vars : 307  bytes / 14%


