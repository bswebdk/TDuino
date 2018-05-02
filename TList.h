/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TList.h   
  
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

#ifndef TLIST_H
#define TLIST_H
#include "TBase.h"

/**
 * \brief List used for handling multiple objects.
 * 
 * TList is used for convenient handling of multiple descendants of TBase.
 * Even though it is neat, you should not use TList unless you would put your
 * objects into an array for easy looping anyway.
 * 
 * TList introduces extra overhead and memory usage which may not be what
 * you want. Using TList for a couple of objects would not really make any
 * sense. Using TList for a lot of objects would probably make sense.
*/
class TList : public TBase {

private:

  class TDummy : public TBase {
  public:
    virtual void loop() {};
    virtual void setup() {};
  } dummy;
  
#if TDUINO_LIST_SIZE > 0
  TBase* ptrList[TDUINO_LIST_SIZE];
  const byte size = TDUINO_LIST_SIZE;
#else
  TBase** ptrList;
  byte size;
#endif

#ifdef TDUINO_DEBUG
  byte memError;
  bool badIndex(byte index, const char *token);
  virtual void detachFrom(TBase* obj);
#endif
  
public:

  /**
	* \brief Constructor for class TList.
	* \param size The size of the list.
	* 
	* Constructs an instance of TList. The _size_ specified should match the
	* number of objects you want to store in the list.
   * 
   * _size_ must be in the range 1..255, memory usage (in bytes): (2 * size) + 2.
   * 
   * \ref static_allocation
	*/
  TList(byte size);
  
  /**
   * \brief Destructor for class TList.
   * 
   * Releases any dynamically allocated memory used for items.
   */
  ~TList();
  
  /**
	* \brief Delete an element.
	* \param index Index of an element to delete.
	* 
	* Removes the element at _index_ without destroying it.
	*/
  void deleteAt(byte index);
  
  /**
	* \brief Check if an index is empty.
	* \param index The index to check.
	* \returns True if the element at _index_ is empty.
	*/
  bool empty(byte index);
  
  /**
	* \brief Find first empty element.
	* \returns The index of the first unused element or -1 if the list is full.
	*/
  int firstEmpty();
  
  /**
	* \brief Get an element.
	* \param index The index of the element to get.
	* \returns The element at specified index or NULL if the index is unoccupied.
	*/
  TBase* get(byte index);

  /**
   * \brief Get the size of the list.
   * \returns The size of the list.
   * 
   * Can be used to determine how many elements the list will be able
   * to hold.
   */
  byte getSize();
  
  /**
	* \brief Release an element.
	* \param index The index of the element to release.
	* 
	* This will remove and destroy the element at the specified index.
	*/
  void releaseAt(byte index);
  
  /**
	* \brief Set an element.
	* \param index The index of the element to set.
	* \param obj The elemnt to set.
	* 
	* This will set / replace the element at _index_ with _obj_. If _obj_ is NULL
	* set() will do the same as deleteAt().
	*/
  void set(byte index, TBase *obj);
  
  /**
	* \brief The list's loop phase.
	* 
	* This method will call the loop() method of all elements in the list.
	*/
  virtual void loop();
  
  /**
	* \brief The list's setup phase.
	* 
	* This method will call the setup() method of all elements in the list.
	*/
  virtual void setup();
  
};

#endif //TLIST_H
