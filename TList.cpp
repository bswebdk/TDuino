/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TList.cpp 
  
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

#include "TList.h"

#ifdef TDUINO_DEBUG
bool TList::badIndex(byte index, const char *token)
{
  if (index < size) return false;
  TDuino_Error(TDUINO_ERROR_BAD_LIST_INDEX, index, token);
  return true;
}

void TList::detachFrom(TBase* obj)
{
  for (byte i = 0; i < this->size; i++) if (ptrList[i] == obj)
  {
    ptrList[i] = &dummy;
    TDuino_Error(TDUINO_ERROR_OBJECT_DIED, i);
  }
}
#endif

TList::TList(byte size) : TBase()
{
#ifdef TDUINO_LIST_SIZE > 0
  //Silence the warning
  size++;
#else
  this->size = (size < 1) ? 1 : size;
  #ifdef TDUINO_DEBUG
    if (freeRam() < ((this->size * 2) + 2))
    {
      this->memError = this->size;
      this->size = 1;
    }
    else this->memError = 0;
  #endif //TDUINO_DEBUG
  this->ptrList = new TBase*[size];
#endif
  for (byte i = 0; i < this->size; i++) ptrList[i] = &dummy;
}

TList::~TList()
{
#if TDUINO_LIST_SIZE > 0
  //Nothing
#else
  delete[] ptrList;
#endif
}

void TList::deleteAt(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("deleteAt"))) return;
  ptrList[index]->attachedTo = NULL;
#endif
  ptrList[index] = &dummy;
}

bool TList::empty(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("empty"))) return false;
#endif
  return ptrList[index] == &dummy;
}

int TList::firstEmpty()
{
  for (int i = 0; i < size; i++) if (ptrList[i] == &dummy) return i;
  return -1;
}

TBase* TList::get(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("get"))) return NULL;
#endif
  return ptrList[index] == &dummy ? NULL : ptrList[index];
}

byte TList::getSize()
{
  return size;
}

void TList::releaseAt(byte index)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("releaseAt"))) return;
#endif
  if (!empty(index))
  {
    TBase *temp = ptrList[index];
    ptrList[index] = &dummy;
    delete temp;
  }
}

void TList::set(byte index, TBase *obj)
{
#ifdef TDUINO_DEBUG
  if (badIndex(index, PSTR("set"))) return;
  obj->attachedTo = this;
#endif
  ptrList[index] = obj == NULL ? &dummy : obj;
}
  
void TList::loop() {
  for (byte i = 0; i < size; i++) ptrList[i]->loop();
}

void TList::setup()
{
#ifdef TDUINO_DEBUG
  if (memError > 0) TDuino_Error(TDUINO_ERROR_NOT_ENOUGH_MEMORY, memError, "TList");
#endif
  for (byte i = 0; i < size; i++) ptrList[i]->setup();
}
