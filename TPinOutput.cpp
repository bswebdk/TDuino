#include "TPinOutput.h"

#define PINTASK_PULSE 1
#define PINTASK_OSCILLATE 2

void TPinOutput::defaults()
{
  TPin::defaults();
  this->task = 0;
}

TPinOutput::TPinOutput()
{
   defaults();
}

void TPinOutput::attach(byte pin, byte mode UNUSED_ATTR)
{
  TPin::attach(pin, OUTPUT);
}

void TPinOutput::counter()
{
#ifdef ENABLE_TIGHT_TIMING
  lastMillis += use_ms;
#else
  lastMillis = loopMillis;
#endif
  if ((stateCur == stateInit) && (repeats > 0) && (++count >= repeats)) stop();
}
  
void TPinOutput::oscillate(unsigned int intervalRising, unsigned int intervalFalling, unsigned int repetitions, int initialState, int stateLowest, int stateHighest)
{
#ifdef TDUINO_DEBUG
  const static char func_tag[] PROGMEM = "oscillate";
  if (!isPwmValid(stateLowest, func_tag) || !isPwmValid(stateHighest, func_tag)) return;
#endif
  this->lastMillis = loopMillis;// - (msLow > msHigh) ? msLow : msHigh;
  this->msLow = intervalFalling;
  this->msHigh = intervalRising;
  this->stateInit = abs(initialState - stateLowest) < abs(initialState - stateHighest) ? stateLowest : stateHighest;
  this->stateCur = this->stateInit;
  this->repeats = repetitions;
  this->count = 0;
  this->stateLow = stateLowest;
  this->stateHigh = stateHighest;
  this->stop();
  this->task = PINTASK_OSCILLATE;
  pwm(this->stateInit);//analogWrite(pin, this->stateInit);
}
void TPinOutput::oscillate(unsigned int intervalRising, unsigned int intervalFalling, unsigned int repetitions, int initialState)
{ oscillate(intervalRising, intervalFalling, repetitions, initialState, 0, 255); }
void TPinOutput::oscillate(unsigned int interval, unsigned int repetitions, int initialState) 
{ oscillate(interval, interval, repetitions, initialState, 0, 255); }
void TPinOutput::oscillate(unsigned int interval, unsigned int repetitions)
{ oscillate(interval, interval, repetitions, 0, 0, 255); }
void TPinOutput::oscillate(unsigned int interval)
{ oscillate(interval, interval, 0, 0, 0, 255); }
  
void TPinOutput::pulse(unsigned int intervalLow, unsigned int intervalHigh, unsigned int repetitions, int initialState)
{
  this->lastMillis = loopMillis;// - (initialState == LOW) ? intervalLow : intervalHigh;
  this->msLow = intervalLow;
  this->msHigh = intervalHigh;
  this->stateCur = initialState & 1;// same as % 2, just faster :)
  this->stateInit = this->stateCur;
  this->repeats = repetitions;
  this->count = 0;
  this->stop();
  this->task = PINTASK_PULSE;
  enable(this->stateInit);
  //digitalWrite(pin, this->stateInit);
}
void TPinOutput::pulse(unsigned int interval, unsigned int repetitions, int initialState) { pulse(interval, interval, repetitions, initialState); }
void TPinOutput::pulse(unsigned int interval, unsigned int repetitions) { pulse(interval, interval, repetitions, HIGH); }
void TPinOutput::pulse(unsigned int interval) { pulse(interval, interval, 0, HIGH); }

void TPinOutput::loop()
{

  TPin::loop();
  
  switch(task) {
    
    case(PINTASK_PULSE):
      use_ms = (stateCur == HIGH) ? msHigh : msLow;
      if (loopMillis - lastMillis >= use_ms)
      {
        stateCur ^= 1;
        enable(stateCur);//digitalWrite(pin, stateCur);
        counter();
      }
      break;
      
    case(PINTASK_OSCILLATE):
      use_ms = (stateCur == stateLow) ? msHigh : msLow;
      unsigned long e = loopMillis - lastMillis;
      if (e >= use_ms) {
        stateCur = (stateCur == stateLow) ? stateHigh : stateLow;
        pwm(stateCur); //analogWrite(pin, stateCur);
        counter();
      } else {
        if (stateCur == stateLow) pwm( /*analogWrite(pin,*/ map(use_ms - e, 0, use_ms - 1, stateHigh, stateLow));
        else pwm( /*analogWrite(pin,*/ map(use_ms - e, 0, use_ms - 1, stateLow, stateHigh));
      }
      break;
  }
  
}

void TPinOutput::stop()
{
  this->task = 0;
}

