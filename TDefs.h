/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TDefs.h   
  
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

#ifndef TDEFS_H
#define TDEFS_H

//Uncomment the following line to enable debug mode
//#define TDUINO_DEBUG

//Uncomment the following line to use 10 statically allocated items per instance of TList
//#define TDUINO_LIST_SIZE 10

//Uncomment the following line to use 10 statically allocated timer slots per instance of TTimer
//#define TDUINO_TIMER_SIZE 10

//Uncomment the following line to use 10 statically allocated timeline slots per instance of TTimeline
//#define TDUINO_TIMELINE_SIZE 10

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifdef TDUINO_DEBUG

/**
 * \file TDefs.h
 * 
 * \defgroup static_allocation Static allocation of object memory
 * 
 * By default TList, TTimer and TTimeline uses dynamic memory allocation for the
 * items / slots they hold. In some cases this may not be an ideal sollution and
 * therefore it is possible to allocate the memory used by the objects in a
 * static way.
 * 
 * You need to modify __TDefs.h__ in the library folder and you must focus on the
 * lines:
 * 
 * \code
 * //#define TDUINO_LIST_SIZE 10
 * //#define TDUINO_TIMER_SIZE 10
 * //#define TDUINO_TIMELINE_SIZE 10
 * \endcode
 * 
 * First you need to uncomment the line which belongs to the object you want to use
 * static allocation for. Next you need to define how many items / slots that object
 * should be able to hold - by default the value is 10, but you can modify this to
 * suit your needs.
 * 
 * By doing this, all instances of the selected object will hold the number of slots
 * / items specified with the define. The value passed to the objects constructor
 * will be ignored.
 * 
 * \code
 * //TDefs.h
 * #define TDUINO_LIST_SIZE 5
 * 
 * //Sketch.ino
 * TList list1(10);
 * TList list2(8);
 * \endcode
 * 
 * The code above will create two instances of TList, both able to hold 5 items - even
 * if another value is passed to the constructor!
 * 
 * @{ @}
 * 
 * \defgroup debug_const TDuino debugging
 * 
 * The TDuino library allows you to compile your sketch with debugging enabled.
 * When debugging is enabled, extra code used for debugging and error handling
 * will be added to the TDuino library. This will increase the size of your
 * compiled sketch but it will also help you detect any errors or bugs caused
 * by incorrect usage of the TDuino library.
 * 
 * In order to enable debugging, you must edit the file __TDefs.h__ in the TDuino
 * library folder and uncomment (remove //) the following line:
 * 
 * \code
 * //#define TDUINO_DEBUG
 * \endcode
 * 
 * The debugging functionality will use "Serial" to print errors and warnings to
 * the console, so you must remember to initialize serial communication when using
 * the TDebug module.
 * 
 * An error message will be displayed as:
 * 
 * \code
 * TDuino_Error E M(I)
 * \endcode
 * 
 * And a warning will be displayed as:
 * 
 * \code
 * TDuino_Warning E M(I)
 * \endcode
 * 
 * Where _E_ is the error/warning number, _M_ is a method name (where possible) and
 * _I_ is the bad index or pin-number (where possible).
 * 
 * Objects using dynamic memory (TList, TTimer & TTimeline) will check for available
 * memory during initialization when debugging is enabled. If there is not enough memory
 * for the amount of items requested, an error will be generated and the amount of items
 * for the corresponding object will be reduced to one.
 * 
 * __NOTE:__ In order to prevent flooding of the console, TDuino's debugging module
 * will only allow repetition of the same error number once every ½ second. This means
 * that some debugging related messages may be omitted.
 * 
 * @{
 */

/**
 * \brief Error related to referencing a bad index in TList.
 */
const PROGMEM byte TDUINO_ERROR_BAD_LIST_INDEX = 1;

/**
 * \brief Error related to referencing a bad index in TTimer.
 */
const PROGMEM byte TDUINO_ERROR_BAD_TIMER_INDEX = 2;

/**
 * \brief Error related to referencing a bad index in TTimeline.
 */
const PROGMEM byte TDUINO_ERROR_BAD_TIMELINE_INDEX = 3;

/**
 * \brief Error caused by an object in TList which is unexpectedly destroyed.
 */
const PROGMEM byte TDUINO_ERROR_OBJECT_DIED = 4;

/**
 * \brief Error used when a parameter passed to a function is invalid.
 */
const PROGMEM byte TDUINO_ERROR_BAD_PARAMETER = 5;

/**
 * \brief Error used when an invalid operation is being performed.
 */
const PROGMEM byte TDUINO_ERROR_INVALID_OPERATION = 6;

/**
 * \brief Error used when trying to allocate more dynamic memory than
 * possible (in TList, TTimer or TTimeline).
 */
const PROGMEM byte TDUINO_ERROR_NOT_ENOUGH_MEMORY = 7;

/**
 * \brief Error used when trying to use an uninitialized instance of TPin
 * or any derived classes (TPinInput, TPinOutput, TButton). Usually caused
 * when the default constructor was used and TPin::attach() was not called
 */
const PROGMEM byte TDUINO_ERROR_INVALID_PIN = 8;

/**
 * \brief Warning used when reading an output pin or writing an input pin.
 */
const PROGMEM byte TDUINO_WARNING_BAD_PINMODE = 100;

/**
 * \brief Warning used when resuming or restarting an active timer or timeline slot.
 */
const PROGMEM byte TDUINO_WARNING_RESUME_ACTIVE = 101;

/**
 * \fn TDuino_Error(byte number, int extra, const char *token = NULL)
 * \brief Print an error with extra info.
 * \param number The error number.
 * \param extra The index / pin number on which the error occurred.
 * \param token The name of the method (or alike) where the error occurred.
 */
void TDuino_Error(byte number, int extra, const char *token = NULL);

/**
 * \overload TDuino_Error(byte number)
 * \brief Print an error number only.
 */
void TDuino_Error(byte number);

/**
 * \fn TDuino_Warning(byte number, int extra, const char *token = NULL)
 * \brief Print a warning with extra info.
 * 
 * Please check TDuino_Error() for information about the arguments.
 */
void TDuino_Warning(byte number, int extra, const char *token = NULL);

/**
 * \overload TDuino_Warning(byte number)
 * \brief Print an warning number only.
 */
void TDuino_Warning(byte number);

/**
 * \brief Method to check free memory.
 * \return Amount of free memory.
 * 
 * This method is used to check the amount of free dynamic memory on the arduino.
 * In TDuino it is used in debugging mode in order to check if there is enough
 * free memory when constructing lists, timers, timelines etc.
 * 
 * The method has been copied from here: https://playground.arduino.cc/Code/AvailableMemory
 * and since improved with guards which will prevent a linkage error due to unfound external
 * values.
 * 
 * For now the function should be compatible with: Uno, Nano, Micro, Leonardo, Mega and
 * NodeMCU (ESP8266).
 */
int freeRam ();

/// @}

#endif //TDUINO_DEBUG
#endif //TDEBUG_H

