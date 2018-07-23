/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TTimer.cpp
  
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

#include "TTimer.h"

#define RESTART(t) t->lastMillis = loopMillis; t->count=0; t->active=true;

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#ifdef TDUINO_DEBUG
bool TTimer::badIndex(byte index, const char* token)
{
  if (index < numTimers) return false;
  TDuino_Error(TDUINO_ERROR_BAD_TIMER_INDEX, index, token);
  return true;
}
#endif
  
TTimer::TTimer(void(*callback)(byte), byte numTimers) : TBase()
{
#if TDUINO_TIMER_SIZE > 0
  //Silence the warning
  numTimers++;
#else
  this->numTimers = (numTimers < 1) ? 1 : numTimers;
  #ifdef TDUINO_DEBUG
    if (freeRam() < ((this->numTimers * 13) + 2))
    {
      this->memError = this->numTimers;
      this->numTimers = 1;
    }
    else this->memError = 0;
  #endif // TDUINO_DEBUG
  this->timers = new TTIMER_SLOT[this->numTimers];
#endif //TDUINO_TIMER_SIZE
  this->callback = callback;
  memset(this->timers, 0, sizeof(TTIMER_SLOT) * this->numTimers);
}

TTimer::~TTimer()
{
#if TDUINO_TIMER_SIZE > 0
  //Nothing
#else
  delete[] this->timers;
#endif
}

int TTimer::firstActive()
{
  for (dummy = 0; dummy < numTimers; dummy++) if (timers[dummy].active) return dummy;
  return -1;
}

int TTimer::firstInactive()
{
  for (dummy = 0; dummy < numTimers; dummy++) if (!timers[dummy].active) return dummy;
  return -1;
}


unsigned int TTimer::getCounter(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("getCount"))) return 0;
#endif
  return timers[index].count;
}

unsigned long TTimer::getInterval(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("getInterval"))) return 0;
#endif
  return timers[index].interval;
}

byte TTimer::getSize()
{
  return numTimers;
}

bool TTimer::isActive(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("isActive"))) return false;
#endif
  return this->timers[index].active;
}

void TTimer::restart(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("restart"))) return;
  if (this->timers[index].active) TDuino_Warning(TDUINO_WARNING_RESUME_ACTIVE, index, PSTR("restart"));
#endif
  current = &this->timers[index];
  RESTART(current);
}

void TTimer::restartAll()
{
  for (dummy = 0; dummy < numTimers; dummy++) restart(dummy);
}

void TTimer::resume(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("resume"))) return;
  if (this->timers[index].active) TDuino_Warning(TDUINO_WARNING_RESUME_ACTIVE, index, PSTR("resume"));
#endif
  //current = &this->timers[index];
  //current->lastMillis = loopMillis;
  //current->active = true;
  this->timers[index].active = true;
}

void TTimer::resumeAll()
{
  for (dummy = 0; dummy < numTimers; dummy++) resume(dummy);
}

void TTimer::set(byte index, unsigned long interval, unsigned int repetitions)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("set"))) return;
#endif
  current = &timers[index];
  current->interval = interval;
  current->repeat = repetitions;
  RESTART(current);
}
void TTimer::set(unsigned long interval, unsigned int repetitions) { set(0, interval, repetitions); }

void TTimer::stop(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("stop"))) return;
#endif
  this->timers[index].active = false;
}

void TTimer::stopAll()
{
  for (dummy = 0; dummy < numTimers; dummy++) stop(dummy);
}

void TTimer::loop()
{
#ifdef TDUINO_DEBUG
  if (memError > 0)
  {
     TDuino_Error(TDUINO_ERROR_NOT_ENOUGH_MEMORY, memError, PSTR("TTimer"));
     memError = 0;
  }
#endif //TDUINO_DEBUG

  TBase::loop();
  for (byte i = 0; i < this->numTimers; i++)
  {
    current = &this->timers[i];
    if (current->active && (loopMillis - current->lastMillis >= current->interval))
    {
      current->lastMillis = loopMillis;
      current->count++;
      (*callback)(i);
      if ((current->repeat > 0) && (current->count >= current->repeat)) current->active = false; 
    }
  }
}

