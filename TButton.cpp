/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TButton.cpp
  
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

#include "TButton.h"

void TButton::defaults()
{
  TPinInput::defaults();
#ifdef TIMING_WITH_MICROS
  debounce = 20000;
#else
  debounce = 20;
#endif
  delay1 = 0;
  delay2 = 0;
  lastRepeat = 0;
}

void TButton::falling()
{
  lastRepeat = loopMillis;
  TPinInput::falling();
}

TButton::TButton()
{
  defaults();
}

void TButton::attach(byte pin, byte mode UNUSED_ATTR)
{
  TPinInput::attach(pin, INPUT_PULLUP);
  this->mode = this->mode & ~ANALOG_BIT; //Clear analog bit
  //analog = false;
}

bool TButton::isPressed()
{
  return (lastState == LOW);
}

void TButton::setRepeat(unsigned int firstDelay, unsigned int followingDelay)
{
  delay1 = firstDelay;
  delay2 = followingDelay;
}

void TButton::onPress(TPinInputCallback callback)
{
  TPinInput::onFalling(callback);
}

void TButton::onRelease(TPinInputCallback callback)
{
  TPinInput::onRising(callback);
}

void TButton::loop()
{
  TPinInput::loop();
  //if pressed and use-delay and elapsed-since-last-repeat > delay then call falling()
  if ((lastState == LOW) && (delay1 | delay2) && (loopMillis - lastRepeat >= ((lastRepeat == changeMillis) ? delay1 : delay2))) falling();
}
