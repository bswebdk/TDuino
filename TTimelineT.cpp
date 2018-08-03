/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TTimelineT.cpp   
  
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

#include "TTimelineT.h"

template <class DATATYPE>
TTimelineT<DATATYPE>::TTimelineT(void(*callback)(byte, DATATYPE), byte numSlots) : TTimeline(NULL, numSlots)
{
  this->callback = callback;
  this->mapMin = 0;
  this->mapMax = sizeof(DATATYPE) == 1 ? 255 : 1023;
}

template <class DATATYPE>
void TTimelineT<DATATYPE>::setMinMax(DATATYPE minValue, DATATYPE maxValue)
{
  this->mapMin = minValue;
  this->mapMax = maxValue;
}

template <class DATATYPE>
void TTimelineT<DATATYPE>::loop()
{
#ifdef TDUINO_DEBUG
  checkMemError(PSTR("TTimelineT"));
#endif //TDUINO_DEBUG

  TBase::loop();
  for (byte i = 0; i < numSlots; i++)
  {
    current = &this->slots[i];
    if (current->state == TL_STATE_ACTIVE)
    {
      DATATYPE p;
      if (loopMillis - current->start >= current->duration)
      {
        p = mapMax;
        current->state = TL_STATE_INACTIVE;
      }
      else p = map(loopMillis - current->start, 0, current->duration - 1, mapMin, mapMax);
      (*callback)(i, p);
    }
    else if ((current->state == TL_STATE_POSTPONED) && (loopMillis - current->start >= current->after))
    {
    #ifdef ENABLE_TIGHT_TIMING
      current->start += current->after;
    #else
      current->start = loopMillis;
    #endif
      current->state = TL_STATE_ACTIVE;
      if (current->duration > 0) (*callback)(i, mapMin); //Make sure that transition starts from mapMin
    } 
  }
}
