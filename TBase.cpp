/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TBase.cpp 
  
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

#include "TBase.h"

void TBase::defaults()
{
#ifdef TDUINO_DEBUG
  this->loopMillis = TDUINO_SIMULATED_UPTIME;
  this->attachedTo = NULL;
#else
  this->loopMillis = 0;
#endif
}

TBase::TBase()
{
  defaults();
}

TBase::~TBase()
{
#ifdef TDUINO_DEBUG
  if (this->attachedTo) this->attachedTo->detachFrom(this);
#endif
}

void TBase::loop() {
#ifdef TIMING_WITH_MICROS
  loopMillis = micros();
#else 
  loopMillis = millis();
#endif
#ifdef TDUINO_DEBUG
  loopMillis += TDUINO_SIMULATED_UPTIME;
#endif
}
