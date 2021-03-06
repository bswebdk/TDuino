/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TPinInput.cpp  
  
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

#include "TPinInput.h"

#ifdef TPININPUT_FLOAT_MATH
  #define SAMPLEDIV(a, b) round((float)a / float(b))
#else
  #define SAMPLEDIV(a, b) a / b;
#endif

#define DEVIFIXED_BIT 128

//Used as a dummy to prevent one or two "if (callback)" statement(s) for each loop.
void dummy_callback(byte b UNUSED_ATTR, int i UNUSED_ATTR) {}

void TPinInput::defaults()
{
  TPin::defaults();
  this->sampleIdx = 0;
  this->sampleVal = 0;
  this->deviation = 1;
  this->samples = 0;
  this->sampleBuffer = NULL;
  this->lastState = 0;
  this->debounce = 0;
  this->changeMillis = 0;
  this->callbacks[0] = /*(void*)*/dummy_callback;//NULL;
  this->callbacks[1] = /*(void*)*/dummy_callback;//NULL;
}

void TPinInput::falling()
{
  callbacks[0](pin, lastState);
}

void TPinInput::rising()
{
  callbacks[1](pin, lastState);
}

TPinInput::TPinInput()
{
  defaults();
}

TPinInput::~TPinInput()
{
  if (sampleBuffer) delete[] sampleBuffer;
}

void TPinInput::attach(byte pin, byte mode)
{
  if ((mode != INPUT) && (mode != INPUT_PULLUP))
  {
  #ifdef TDUINO_DEBUG
    if ((mode != INPUT) && (mode != INPUT_PULLUP)) TDuino_Error(TDUINO_ERROR_BAD_PARAMETER, mode, PSTR("TPinInput::mode"));
  #endif //TDUINO_DEBUG
    mode = INPUT;
  }
  TPin::attach(pin, mode);
  lastState = read();
}

int TPinInput::read(byte samples)
{
#ifdef TDUINO_DEBUG
  if (samples < 2)
  {
     TDuino_Error(TDUINO_ERROR_BAD_PARAMETER, samples, PSTR("TPinInput::read"));
     return -1;
  }
#else
  if (samples < 2) return -1;
#endif //TDUINO_DEBUG

#ifdef PININPUT_FLOAT
  float res = 0;
#else
  unsigned long res = 0;
#endif

  if (debounce > 0)
  {
    if (mode & ANALOG_BIT) for (dummy = 0; dummy < samples; dummy++) { delay(debounce); res += analogRead(pin); }
    else for (dummy = 0; dummy < samples; dummy++) { delay(debounce); res += digitalRead(pin); }
  }
  else
  {
    if (mode & ANALOG_BIT) for (dummy = 0; dummy < samples; dummy++) res += analogRead(pin);
    else for (dummy = 0; dummy < samples; dummy++) res += digitalRead(pin);
  }
  return SAMPLEDIV(res, samples); //round(res / samples);
}

unsigned int TPinInput::getDebounce()
{
  return debounce;
}

void TPinInput::setDebounce(unsigned int debounce)
{
  this->debounce = debounce;
}

byte TPinInput::getDeviation()
{
  return deviation & 127;
}

void TPinInput::setDeviation(byte deviation, bool fixedSteps)
{
  if ((deviation > 1) and (!isAnalog())) deviation = 1; 
  this->deviation = deviation;
  this->fixedDeviation = fixedSteps;
}

int TPinInput::getBufferedValue(byte sampleIndex)
{
#ifdef TDUINO_DEBUG
   if (!sampleBuffer)
   {
      TDuino_Error(TDUINO_ERROR_INVALID_OPERATION, sampleIndex, PSTR("TPinInput::sampleBuffer"));
      return -1;
   } else if (sampleIndex && (sampleIndex > samples)) TDuino_Error(TDUINO_ERROR_BAD_PARAMETER, sampleIndex, PSTR("TPinInput::getBufferedValue"));
#endif
   return sampleIndex ? sampleBuffer[sampleIndex - 1] : SAMPLEDIV(sampleVal, samples);// round((float)sampleVal / samples);
}

byte TPinInput::getSamples()
{
  return samples;
}

void TPinInput::setSamples(byte samples, bool buffered)
{
#ifdef TDUINO_DEBUG
  if ((samples < 2) || (samples > 60))
  {
     TDuino_Error(TDUINO_ERROR_BAD_PARAMETER, samples, PSTR("TPinInput::setSamples"));
     samples = 0;
  }
#else
  if ((samples < 2) || (samples > 60)) samples = 0;
#endif
  if ((this->samples == samples) && ((sampleBuffer != NULL) != buffered)) return;
  this->samples = samples;
  if (sampleBuffer) delete[] sampleBuffer;
  if ((!buffered) || (samples == 0)) sampleBuffer = NULL;
  else
  {
    sampleBuffer = new int[samples];
    sampleIdx = 0;
    sampleVal = 0;
    for (dummy = 0; dummy < samples; dummy ++)
    {
       sampleBuffer[dummy] = read();
       sampleVal += sampleBuffer[dummy];
    }
  }
}

void TPinInput::loop()
{
  
  TPin::loop();
  
  #ifdef ENABLE_TIGHT_TIMING
  #define CHMS() changeMillis += debounce
  #else
  #define CHMS() changeMillis = loopMillis
  #endif

  if (loopMillis - changeMillis >= debounce) {

    if (sampleBuffer)
    {
      CHMS();
      sampleVal -= sampleBuffer[sampleIdx];
      sampleBuffer[sampleIdx] = read();
      sampleVal += sampleBuffer[sampleIdx];
      if (++sampleIdx == samples) sampleIdx = 0;
      dummy = SAMPLEDIV(sampleVal, samples);
      //dummy = round((float)sampleVal / (float)samples); //getBufferedValue();
    }
    else if (samples == 0) dummy = read();
    else
    {
      sampleVal += read();
      if (++sampleIdx == samples)
      {
        dummy = SAMPLEDIV(sampleVal, samples);
        //dummy = round((float)sampleVal / (float)samples);
        sampleVal = 0;
        sampleIdx = 0;
      }
      else
      {
        CHMS();
        return;
      }
    }
    
    if (dummy >= (lastState + deviation))
    {
      lastState = (fixedDeviation && (deviation > 0)) ? lastState + deviation : dummy;
      rising();
    }
    else if (dummy <= (lastState - deviation))
    {
      lastState = (fixedDeviation && (deviation > 0)) ? lastState - deviation : dummy;
      falling();
    }
    else return;
    
    CHMS();
    
  }

}

void TPinInput::onFalling(TPinInputCallback callback)
{
  callbacks[0] = callback ? callback : dummy_callback;
}

void TPinInput::onRising(TPinInputCallback callback)
{
  callbacks[1] = callback ? callback : dummy_callback;
}

