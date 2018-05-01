#include "TPinOutput.h"

#define PINTASK_PULSE 1
#define PINTASK_OSCILLATE 2

TPinOutput::TPinOutput(byte pin, byte mode) : TPin(pin, mode)
{
   this->task = 0;
}

void TPinOutput::counter()
{
  lastMillis = loopMillis;
  if ((stateCur == stateInit) && (repeats > 0) && (++count >= repeats)) stop();
}
  
void TPinOutput::oscillate(unsigned int intervalRising, unsigned int intervalFalling, unsigned int repetitions, byte initialState, byte stateLowest, byte stateHighest)
{
  this->lastMillis = loopMillis;// - (msLow > msHigh) ? msLow : msHigh;
  this->msLow = intervalFalling;
  this->msHigh = intervalRising;
  this->stateInit = abs((int)initialState - stateLowest) < abs((int)initialState - stateHighest) ? stateLowest : stateHighest;
  this->stateCur = this->stateInit;
  this->repeats = repetitions;
  this->count = 0;
  this->stateLow = stateLowest;
  this->stateHigh = stateHighest;
  this->stop();
  this->task = PINTASK_OSCILLATE;
}
void TPinOutput::oscillate(unsigned int intervalRising, unsigned int intervalFalling, unsigned int repetitions, byte initialState)
{ oscillate(intervalRising, intervalFalling, repetitions, initialState, 0, 255); }
void TPinOutput::oscillate(unsigned int interval, unsigned int repetitions, byte initialState) 
{ oscillate(interval, interval, repetitions, initialState, 0, 255); }
void TPinOutput::oscillate(unsigned int interval, unsigned int repetitions)
{ oscillate(interval, interval, repetitions, 0, 0, 255); }
void TPinOutput::oscillate(unsigned int interval)
{ oscillate(interval, interval, 0, 0, 0, 255); }
  
void TPinOutput::pulse(unsigned int intervalLow, unsigned int intervalHigh, unsigned int repetitions, byte initialState)
{
  this->lastMillis = loopMillis - (initialState == LOW) ? intervalLow : intervalHigh;
  this->msLow = intervalLow;
  this->msHigh = intervalHigh;
  this->stateCur = initialState;
  this->stateInit = initialState;
  this->repeats = repetitions;
  this->count = 0;
  this->stop();
  this->task = PINTASK_PULSE;
}
void TPinOutput::pulse(unsigned int interval, unsigned int repetitions, byte initialState) { pulse(interval, interval, repetitions, initialState); }
void TPinOutput::pulse(unsigned int interval, unsigned int repetitions) { pulse(interval, interval, repetitions, HIGH); }
void TPinOutput::pulse(unsigned int interval) { pulse(interval, interval, 0, HIGH); }

void TPinOutput::loop()
{

  TPin::loop();
  
  switch(task) {
    
    case(PINTASK_PULSE):
      if (loopMillis - lastMillis >= (stateCur == HIGH ? msHigh : msLow))
      {
        stateCur ^= 1;
        digitalWrite(pin, stateCur);
        counter();
      }
      break;
      
    case(PINTASK_OSCILLATE):
      unsigned int e = loopMillis - lastMillis;
      unsigned int i = stateCur == stateLow ? msHigh : msLow;
      if (e >= i) {
        stateCur = (stateCur == stateLow) ? stateHigh : stateLow;
        analogWrite(pin, stateCur);
        counter();
      } else {
        int v = round((float)(stateHigh - stateLow) * ((float)e / (float)i));
        if (stateCur == stateHigh) v = stateHigh - v;
        else v += stateLow;
        analogWrite(pin, v);
      }
      break;
  }
  
}

void TPinOutput::stop()
{
  this->task = 0;
}

