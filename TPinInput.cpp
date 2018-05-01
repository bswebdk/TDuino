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

//Used as a dummy to prevent one or two "if (callback)" statement(s) for each loop.
void dummy_callback(byte b, int i) {}

void TPinInput::falling()
{
  //if (callbacks[0]) ( ( void (*) (byte, int) ) callbacks[0])(pin, state);
  //( ( void (*) (byte, int) ) callbacks[0])(pin, lastState);
  callbacks[0](pin, lastState);
}

void TPinInput::rising()
{
  //if (callbacks[1]) ( ( void (*) (byte, int) ) callbacks[1])(pin, state);
  //( ( void (*) (byte, int) ) callbacks[1])(pin, lastState);
  callbacks[1](pin, lastState);
}

TPinInput::TPinInput(byte pin, byte mode) : TPin(pin, mode)
{
#ifdef TDUINO_DEBUG
  if ((mode != INPUT) && (mode != INPUT_PULLUP)) TDuino_Error(TDUINO_ERROR_BAD_PARAMETER, mode, PSTR("TPinInput::mode"));
#endif
  this->sampleIdx = 0;
  this->sampleVal = 0;
  this->samples = 0;
  this->sampleBuffer = NULL;
  this->lastState = 0;
  this->debounce = 0;
  this->changeMillis = 0;
  this->callbacks[0] = /*(void*)*/dummy_callback;//NULL;
  this->callbacks[1] = /*(void*)*/dummy_callback;//NULL;
}
  
TPinInput::~TPinInput()
{
  if (sampleBuffer) delete[] sampleBuffer;
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
#endif
  float res = 0;
  if (debounce > 0)
  {
    if (analog) for (dummy = 0; dummy < samples; dummy++) { delay(debounce); res += analogRead(pin); }
    else for (dummy = 0; dummy < samples; dummy++) { delay(debounce); res += digitalRead(pin); }
  }
  else
  {
    if (analog) for (dummy = 0; dummy < samples; dummy++) res += analogRead(pin);
    else for (dummy = 0; dummy < samples; dummy++) res += digitalRead(pin);
  }
  return round(res / samples);
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
  return deviation;
}

void TPinInput::setDeviation(byte deviation)
{
  this->deviation = deviation;
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
   return sampleIndex ? sampleBuffer[sampleIndex - 1] : round((float)sampleVal / samples);
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
  
  if (loopMillis - changeMillis >= debounce) {

    if (sampleBuffer)
    {
       changeMillis = loopMillis;
       sampleVal -= sampleBuffer[sampleIdx];
       sampleBuffer[sampleIdx] = read();
       sampleVal += sampleBuffer[sampleIdx];
       if (++sampleIdx == samples) sampleIdx = 0;
       dummy = round((float)sampleVal / (float)samples); //getBufferedValue();
    }
    else
    {
       
      if (samples == 0) dummy = read();
      else
      {
        sampleVal += read();
        if (++sampleIdx == samples)
        {
           dummy = round((float)sampleVal / samples);
           sampleVal = 0;
           sampleIdx = 0;
        }
        else
        {
          changeMillis = loopMillis;
          return;
        }
      }
    }
    
    if (dummy > (lastState + deviation))
    {
      lastState = dummy;
      rising();
    }
    else if (dummy < (lastState - deviation))
    {
      lastState = dummy;
      falling();
    }
    else return;
    
    changeMillis = loopMillis;
    
  }

}

void TPinInput::setup()
{
  TPin::setup();
  lastState = read();
}

void TPinInput::onFalling(TPinInputCallback callback)
{
  callbacks[0] = callback ? callback : dummy_callback;
}

void TPinInput::onRising(TPinInputCallback callback)
{
  callbacks[1] = callback ? callback : dummy_callback;
}

