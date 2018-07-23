/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TTimeline.h   
  
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

#ifndef TTIMELINE_H
#define TTIMELINE_H

#include "TBase.h"

#define TL_STATE_INACTIVE 0
#define TL_STATE_ACTIVE 1
#define TL_STATE_POSTPONED 2

/**
 * \file TTimeline.h
 * \defgroup TL_HELPERS Timeline helpers
 * 
 * These functions will help you to convert the progress value passed to
 * the timeline callback to numeric values of different types. All functions
 * takes these three arguments:
 * 
 * \b progress The float passed to the callback.
 * \b min The minimum value it should be mapped to.
 * \b max The maximum value it should be mapped to.
 * 
 * The formula is pretty simple: roundf((float)(max - min) * progress) + min
 * 
 * The are functions for signed and unsigned int + long and for float:
 * 
 * \code
 * void TimelineCallback(byte slot, float progress)
 * {
 *   //Map to int value between 1000 and 2000:
 *   int intValue = TL_MapToInt(progress, 1000, 2000);
 * 
 *   //Map to long value between 10000 and 20000:
 *   long longValue = TL_MapToLong(progress, 10000, 20000);
 * 
 *   //Map to float value between 1.5 and 6.5:
 *   float floatValue = TL_MapToFloat(progress, 1.5f, 6.5f);
 * }
 * 
 * TTimeline tline(TimelineCallback, 2);
 * \endcode
 * 
 * @{
 */

/**
 * \brief Map to int
 */
int TL_MapToInt(float progress, int low, int high);

/**
 * \brief Map to unsigned int
 */
unsigned int TL_MapToUInt(float progress, unsigned int low, unsigned int high);

/**
 * \brief Map to long
 */
long TL_MapToLong(float progress, long low, long high);

/**
 * \brief Map to unsigned long
 */
unsigned long TL_MapToULong(float progress, unsigned long low, unsigned long high);

/**
 * \brief Map to float
 */
float TL_MapToFloat(float progress, float low, float high);

/// @}

/// \cond HIDDEN_FIELD

struct TTIMELINE_SLOT
{
  unsigned long after, start, duration;
  byte state;
};

/// \endcond

/**
 * \brief Used to track actions on a virtual time line.
 * 
 * TTimeline is usefull when you need to have sort of a virtual time line
 * where different things are happening gradually in a period of time which
 * can be postponed.
 * 
 * \code
 * void TimelineCallback(byte slot, float progress)
 * {
 *   // "slot" is the index of the slot in the time line.
 *   // "progress" is the amount of progress from 0.0f (start) to 1.0f (end).
 * }
 * 
 * TTimeline tline(TimelineCallback, 2); //Two slots
 * 
 * //Set first slot to start after 5 seconds with a duration of 1 second
 * tline.set(0, 1000, 5000);
 * 
 * //Set the second slot to start after 10 seconds with a duration of 5 seconds
 * tline.set(1, 5000, 10000);
 * \endcode
 * 
 * See \ref TL_HELPERS for a list of helpers for handling the progres value.
 * 
 * <b>IMPORTANT NOTE!</b> For most situations you would use the template class
 * \ref TTimelineT because it is more efficient (no floating points). You should
 * only use TTimeline in situations where you need to translate the progress to
 * multiple different values.
 * 
 * \see TTimelineT
 */
class TTimeline : public TBase
{
private:
  void (*callback)(byte, float);
  
protected:
#if TDUINO_TIMELINE_SIZE > 0
  const byte numSlots = TDUINO_TIMELINE_SIZE;
  TTIMELINE_SLOT slots[TDUINO_TIMELINE_SIZE];
#else
  byte numSlots;
  TTIMELINE_SLOT *slots;
#endif
  
  TTIMELINE_SLOT *current;
  byte dummy;

#ifdef TDUINO_DEBUG
  bool badIndex(byte i, const char *token);
  void checkMemError(const char *token);
  byte memError;
#endif
  
public:
  
  /**
   * \brief The constructor for a TTimeline.
   * \param callback The callback which handles time line events.
   * \param numSlots The number of slots used by this instance.
   * 
   * The callback will be called for each active slot and to it will be passed an
   * index of the slot being handled and the amount of progress for the slot.
   * 
   * _numSlots_ must be in the range 1..255, memory usage (in bytes) is: (13 * numSlots) + 2.
   * 
   * \ref static_allocation
   */
  TTimeline(void(*callback)(byte,float), byte numSlots = 1);
  
  /**
   * \brief The destuctor for a TTimeline
   * 
   * The destructor will release any memory used for slots
   */
  ~TTimeline();

  /**
   * \brief Find index of the first active slot.
   * \returns The index of the first active slot.
   * 
   * If no slots are active, -1 will be returned.
   */
  int firstActive();

  /**
   * \brief Find index of the first inactive slot.
   * \returns The index of the first inactive slot.
   * 
   * If no slots are inactive, -1 will be returned.
   */
  int firstInactive();
  
  /**
	* \brief Get number of timeline slots.
	* \return The number of timeline slots.
	* 
	* Used to get the number of timeline slots used with this instance of
	* TTimeline.
	* 
	* \see TTimeline()
	*/
  byte getSize();

  /**
   * \brief Check if a slot is active.
   * \param index The index of the slot to check.
   * \return true if the slot is active.
   * 
   * Used to check if a slot is active or postponed (waiting to become active).
   */
  bool isActive(byte index);
  
  /**
   * \brief Check if a slot is started.
   * \param index The index of the slot to check.
   * \return true if the slot is started.
   * 
   * Used to check if a slot is started (transition in progress).
   */
  bool isStarted(byte index);

  /**
   * \brief Used to restart a slot.
   * \param index The index of the slot to restart.
   * 
   * This method will restart the slot at _index_ with the values previously
   * set with set().
   */
  void restart(byte index);
  
  /**
   * \brief Used to restart all slots.
   * 
   * This method will restart all slots in the time line.
   */
  void restartAll();
  
  /**
   * \brief Set a slot in the time line.
   * \param index Index of the slot to set.
   * \param duration The duration of the slot.
   * \param startAfter The amount of time (milliseconds) to wait before starting the slot.
   * 
   * Sets the slot at _index_ to start after _startAfter_ milliseconds with _duration_
	* milliseconds. By default _startAfter_ is set to 0, which means that the slot will
	* start emmediately.
	* 
	* If _duration_ is zero, the slot will act as a one-shot timer.
   */
  void set(byte index, unsigned long duration, unsigned long startAfter = 0);
  
  /**
   * \brief Stop a slot.
   * \param index Index of the slot to stop.
   * 
   * This will deactivate the slot at the specified index.
   */
  void stop(byte index);
  
  /**
   * \brief Stop all slots.
   * 
   * This will deactivate all slots in the time line.
   */
  void stopAll();
  
  /**
   * \brief The TTimeline's loop phase.
   * 
   * Must be called for each loop in the sketch.
   * 
   * \see TBase::loop()
   */
  virtual void loop();
  
};

#endif //TTIMELINE_H
