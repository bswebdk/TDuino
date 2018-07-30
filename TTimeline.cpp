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

#define MAP_PCT(pct, low, high) roundf((float)(high - low) * pct) + low
#define RESTART(t) t->start = loopMillis; t->state = (t->after == 0) ? TL_STATE_ACTIVE : TL_STATE_POSTPONED

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

void TTimeline::checkMemError(const char *token)
{
  if (memError > 0)
  {
     TDuino_Error(TDUINO_ERROR_NOT_ENOUGH_MEMORY, memError, token);
     memError = 0;
  }
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

char TTimeline::hasOverlap(byte index1, byte index2, bool remove)
{
#ifdef TDUINO_DEBUG
  const static char func_tag[] PROGMEM = "hasOverlap";
  if (badIndex(index1, func_tag) || badIndex(index2, func_tag)) return 0;
#endif
  
  current = &this->slots[index1];
  TTIMELINE_SLOT *tls = &this->slots[index2];
  if ((current->state == TL_STATE_INACTIVE) || (tls->state == TL_STATE_INACTIVE)) return 0;

#ifdef ENABLE_64BIT
  uint64_t v1 = current->start, v2 = tls->start;
#else
  #ifndef DISABLE_32BIT_ROLLOVER_CHECKS
    #define RO_CHECK
  #endif
  unsigned long v1, v2, st;
  //If using 32 bit, subtract the lowest starting value in order to reduce the risk of rollover
  st = (current->start < tls->start) ? current->start : tls->start;
  v1 = current->start - st;
  v2 = tls->start - st;
#endif
  
  //Add postponation
  if (current->state == TL_STATE_POSTPONED)
  {
  #ifdef RO_CHECK
    if (v1 + current->after < v1) return -1; //Rollover
  #endif
    v1 += current->after;
  }

  if (tls->state == TL_STATE_POSTPONED)
  {
  #ifdef RO_CHECK
    if (v2 + tls->after < v2) return -2; //Rollover
  #endif
    v2 += tls->after;
  }

#ifdef RO_CHECK
  //Make sure that slot duration does not cause rollover
  if (v1 + current->duration < v1) return -1;
  if (v2 + tls->duration < v2) return -2;
#endif

  //Find out which starts first
  
  if (v1 == v2) //Starts simultaneously, return shortest as overlapping
    dummy = (current->duration > tls->duration) ? 2 : 1;
    
  else if (v1 < v2) // 1 starts first
    dummy = ((v1 + current->duration) <= v2) ? 0 : 2; //1 ends before 2 starts : 2 overlaps
    
  else // 2 starts first
    dummy = ((v2 + tls->duration) <= v1) ? 0 : 1; //2 ends before 1 starts : 1 overlaps
  
  if (remove)
  {
      
    //Remove overlap if possible
    if (dummy == 1)
    {
      //1 must be postponed
    #ifdef ENABLE_64BIT
      v1 = (v2 + tls->duration) - current->start;
      if (v1 > 0xFFFFFFFFUL) return -1; //Exceeds 32 bit limits
    #else
      v1 = (v2 + tls->duration) - (current->start - st);
    #endif
      /*Serial.print("1 from ");
      Serial.print(current->after);
      Serial.print(" to ");
      Serial.println(v1);*/
      current->after = v1;
    }
      
    else if (dummy == 2)
    {
      //2 must be postponed
    #ifdef ENABLE_64BIT
      v2 = (v1 + current->duration) - tls->start;
      if (v2 > 0xFFFFFFFFUL) return -2; //Exceeds 32 bit limits
    #else
      v2 = (v1 + current->duration) - (tls->start - st);
    #endif
      /*Serial.print("2 from ");
      Serial.print(tls->after);
      Serial.print(" to ");
      Serial.println(v2);*/
      tls->after = v2;
    }
      
  }
  return dummy;
}

bool TTimeline::isActive(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("isActive"))) return false;
#endif
  return (slots[index].state == TL_STATE_ACTIVE) || (slots[index].state == TL_STATE_POSTPONED);
}

bool TTimeline::isStarted(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("isStarted"))) return false;
#endif
  return slots[index].state == TL_STATE_ACTIVE;
}

void TTimeline::restart(byte index)
{
#ifdef TDUINO_DEBUG
  const static char func_tag[] PROGMEM = "restart";
  if (badIndex(index, func_tag)) return;
  current = &slots[index];
  if (current->state != TL_STATE_INACTIVE) TDuino_Warning(TDUINO_WARNING_RESUME_ACTIVE, index, func_tag);
#else
  current = &slots[index];
#endif
  RESTART(current);
}

void TTimeline::restartAll()
{
  for (dummy = 0; dummy < numSlots; dummy++) restart(dummy);
}

void TTimeline::set(byte index, unsigned long duration, unsigned long startAfter)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("set"))) return;
#endif
  current = &slots[index];
  current->after = startAfter;
  current->duration = duration;
  //current->start = loopMillis;
  //current->state = (startAfter == 0) ? TL_STATE_ACTIVE : TL_STATE_POSTPONED;
  RESTART(current);
}

void TTimeline::stop(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("stop"))) return;
#endif
  slots[index].state = TL_STATE_INACTIVE;
}

void TTimeline::stopAll()
{
  for (dummy = 0; dummy < numSlots; dummy++) stop(dummy);
}

void TTimeline::loop()
{
#ifdef TDUINO_DEBUG
  checkMemError(PSTR("TTimeline"));
#endif //TDUINO_DEBUG

  TBase::loop();
  for (byte i = 0; i < numSlots; i++)
  {
    current = &this->slots[i];
    if (current->state == TL_STATE_ACTIVE)
    {
      float p = (current->duration == 0) ? 1.0f : (float)(loopMillis - current->start) / (float)current->duration;
      if (p >= 1.0f)
      {
        p = 1.0f;
        current->state = TL_STATE_INACTIVE;
      }
      (*callback)(i, p);
    }
    else if ((current->state == TL_STATE_POSTPONED) && (loopMillis - current->start >= current->after))
    {
      current->start = loopMillis;
      current->state = TL_STATE_ACTIVE;
      if (current->duration > 0) (*callback)(i, 0.0f); //Make sure that transition starts from 0.0f
    } 
  }
}
