/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TTimeline.cpp   
  
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

#include "TTimeline.h"

#define STATE_INACTIVE 0
#define STATE_ACTIVE 1
#define STATE_POSTPONED 2

#define MAP_PCT(pct, low, high) roundf((float)(high - low) * pct) + low
#define RESTART(t) t->start = loopMillis; t->state = (t->after == 0) ? STATE_ACTIVE : STATE_POSTPONED

int TL_MapToInt(float progress, int low, int high) { return MAP_PCT(progress, low, high); }
unsigned int TL_MapToUInt(float progress, unsigned int low, unsigned int high) { return MAP_PCT(progress, low, high); }
long TL_MapToLong(float progress, long low, long high) { return MAP_PCT(progress, low, high); }
unsigned long TL_MapToLong(float progress, unsigned long low, unsigned long high) { return MAP_PCT(progress, low, high); }
float TL_MapToFloat(float progress, float low, float high) { return ((high - low) * progress) + low; }

#ifdef TDUINO_DEBUG
bool TTimeline::badIndex(byte i, const char *token)
{
  if (i >= numSlots) {
    TDuino_Error(TDUINO_ERROR_BAD_TIMELINE_INDEX, i, token);
    return true;
  }
  return false;
}
#endif
  
TTimeline::TTimeline(void(*callback)(byte,float), byte numSlots) : TBase()
{
#if TDUINO_TIMELINE_SIZE > 0
  //Silence the warning
  numSlots++;
#else  
  this->numSlots = (numSlots < 1) ? 1 : numSlots;
  #ifdef TDUINO_DEBUG
    if (freeRam() < ((this->numSlots * 13) + 2))
    {
      this->memError = this->numSlots;
      this->numSlots = 1;
    }
    else this->memError = 0;
  #endif // TDUINO_DEBUG
  this->slots = new TTIMELINE_SLOT[this->numSlots];
#endif //TDUINO_TIMELINE_SIZE
  this->callback = callback;
  memset(this->slots, 0, sizeof(TTIMELINE_SLOT) * this->numSlots);
}

TTimeline::~TTimeline()
{
#if TDUINO_TIMELINE_SIZE > 0
  //Nothing
#else
  delete[] this->slots;
#endif
}

int TTimeline::firstActive()
{
  for (dummy = 0; dummy < numSlots; dummy++) if (isActive(dummy)) return dummy;
  return -1;
}

int TTimeline::firstInactive()
{
  for (dummy = 0; dummy < numSlots; dummy++) if (!isActive(dummy)) return dummy;
  return -1;
}

byte TTimeline::getSize()
{
  return numSlots;
}

bool TTimeline::isActive(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("isActive"))) return false;
#endif
  return (slots[index].state == STATE_ACTIVE) || (slots[index].state == STATE_POSTPONED);
}

bool TTimeline::isStarted(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("isStarted"))) return false;
#endif
  return slots[index].state == STATE_ACTIVE;
}

void TTimeline::restart(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("restart"))) return;
  current = &slots[index];
  if (current->state != STATE_INACTIVE) TDuino_Warning(TDUINO_WARNING_RESUME_ACTIVE, index, PSTR("restart"));
#else
  current = &slots[index];
#endif
  RESTART(current);
}

void TTimeline::set(byte index, unsigned long duration, unsigned long startAfter)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("set"))) return;
#endif
  current = &slots[index];
  current->after = startAfter;
  current->duration = duration;
  RESTART(current);
}

void TTimeline::stop(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("stop"))) return;
#endif
  slots[index].state = STATE_INACTIVE;
}

void TTimeline::loop()
{
  TBase::loop();
  for (byte i = 0; i < numSlots; i++)
  {
    current = &this->slots[i];
    if (current->state == STATE_ACTIVE)
    {
      float p = (current->duration == 0) ? 1.0f : (float)(loopMillis - current->start) / (float)current->duration;
      if (p > 1.0f)
      {
        p = 1.0f;
        current->state = STATE_INACTIVE;
      }
      (*callback)(i, p);
    }
    else if ((current->state == STATE_POSTPONED) && (loopMillis - current->start >= current->after))
    {
      current->start = loopMillis;
      current->state = STATE_ACTIVE;
      if (current->duration > 0) (*callback)(i, 0.0f); //Make sure that transition starts from 0.0f
    } 
  }
}

void TTimeline::setup()
{
#ifdef TDUINO_DEBUG
  if (memError > 0) TDuino_Error(TDUINO_ERROR_NOT_ENOUGH_MEMORY, memError, "TTimeline");
#endif
}
