/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TTimer.h  
  
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

#ifndef TTIMER_H
#define TTIMER_H
#include "TBase.h"

/// \cond HIDDEN_FIELD

struct TTIMER_SLOT
{
  unsigned long interval, lastMillis;
  unsigned int repeat, count;
  bool active;
};

/// \endcond

/**
 * \brief Used for timing a callback method.
 * 
 * TTimer is used to perform timed operations using a callback method. It is possible to
 * have up to 255 timed operations (if memory allows it) using one TTimer instance. All
 * operations use the same callback method and passes the index of the timer operation
 * to the callback.
 * 
 * \code
 * void timerCallback(byte timerIndex)
 * {
 *   // "timerIndex" is the index of the timer slot.
 *   if (timerIndex == 0)
 *   {
 *     //Do something every 500 milliseconds
 *   }
 *   else if (timerIndex == 1)
 *   {
 *     //Do something every 1000 milliseconds
 *   }
 * }
 * 
 * TTimer timer(timerCallback, 2);
 * 
 * void setup()
 * {
 *   timer.set(0, 500, 0);
 *   timer.set(1, 1000, 0);
 * }
 * 
 * void loop()
 * {
 *   timer.loop();
 * }
 * \endcode
*/
class TTimer : public TBase {
  
private:
#if TDUINO_TIMER_SIZE > 0
  const byte numTimers = TDUINO_TIMER_SIZE;
  TTIMER_SLOT timers[TDUINO_TIMER_SIZE];
#else
  byte numTimers;
  TTIMER_SLOT* timers;
#endif
  
  TTIMER_SLOT* current;
  void (*callback)(byte);
  byte dummy;
  
#ifdef TDUINO_DEBUG
  bool badIndex(byte index, const char* token);
  byte memError;
#endif
  
public:

  /**
   * \brief Constructs a timer instance.
   * \param callback The callback used by this instance.
   * \param numTimers The number of timer slots to create.
   * 
   * Constructs an instance of the TTimer with a predefined number of timer slots and a
	* callback to be invoked whenever a timer slot is triggered.
   * 
   * _numTimers_ must be in the range 1..255, memory usage (in bytes) is: (13 * numTimers) + 2.
   * 
   * \ref static_allocation
   */
  TTimer(void(*callback)(byte), byte numTimers = 1);
  
  /**
	* \brief Destructor for the TTimer instance.
	* 
	* The destructor releases dynamic memory (if any) used by the TTimer
	* instance.
	*/
  virtual ~TTimer();

  /**
   * \brief Get first active timer slot.
   * 
   * Get the index of the first active timer slot.
   * 
   * \returns The index of the first active timer slot or -1 if all are inactive.
   */
  int firstActive();

  /**
   * \brief Get first inactive timer slot.
   * 
   * Get the index of the first inactive timer slot.
   * 
   * \returns The index of the first inactive timer slot or -1 if all are active.
   */
  int firstInactive();

  /**
   * \brief Get the counter for a timer slot.
   * \param index The index of a timer slot.
   * 
   * The counter for a timer slot is incremented for each trigger. You can use the
   * counter value in order to perform different tasks on different counts. Beware
   * of the counters roll-over at 65535 (0xFFFF).
   * 
   * \returns The counter for the timer slot.
   */
  unsigned int getCounter(byte index = 0);
  
  /**
	* \brief Get the interval for a timer slot.
	* \param index The index of a timer slot.
	* 
	* Get the interval of the timer slot at the specified index.
	* 
	* \returns The interval used by the slot.
	*/
  unsigned long getInterval(byte index = 0);

  /**
   * \brief Get number of timer slots. 
   * \return The number of timer slots.
   * 
   * Used to detect the number of timer slots used by this instance of TTimer.
   */
  byte getSize();
  
  /**
	* \brief Check if a timer slot is active.
	* \param index The index of the timer slot to check.
	* 
	* Check wheter or not a timer slot is active. It will return true
	* if the counter for the slot has not reached the maximum repetitions
	* and if the timer slot has not been stopped.
	* 
	* \see stop()
	*/
  bool isActive(byte index = 0);
  
  /**
   * \brief Restarts a timer slot.
   * \param index The index of the timer slot to restart.
   * 
   * Used to restart a timer slot with the values previosly defined with
   * set().
   * 
   * \see set() stop()
   */
  void restart(byte index = 0);
  
  /**
   * \brief Restarts all timer slot.
   * 
   * Used to restart all timer slots.
   * 
   * \see retart()
   */
  void restartAll();
  
  /**
   * \brief Resumes a timer slot.
   * \param index The index of the timer slot to resume.
   * 
   * Used to resume a timer slot. Unlike #restart(), resume() does not reset
   * the values in the timer slot but resumes from the point where the slot
   * was stopped.
   * 
   * \see restart() stop()
   */
  void resume(byte index = 0);
  
  /**
   * \brief Resumes all timer slots.
   * 
   * Used to resume all timer slots.
   * 
   * \see resume()
   */
  void resumeAll();
  
  /**
	* \fn set(byte index, unsigned long interval, unsigned int repetitions)
	* \brief Set a timer slot.
	* \param index The index of the slot to set.
	* \param interval The interval in which the slot triggers.
	* \param repetitions Maximum number of repetitions to trigger.
	* 
	* Used to set the timer slot at the given index to be triggered with the
	* given interval and the number of repetitions. When the maximum number of
	* repetitions has been triggered, the slot is deactivated. If repetitions
	* is set to 0 (the default), the slot will be triggered indefinately. The
   * first trigger will happen after _interval_ has elapsed.
   */
  void set(byte index, unsigned long interval, unsigned int repetitions);
  
  /**
  * Set the first timer slot.
  * \overload set(unsigned long interval, unsigned int repetitions)
  */
  void set(unsigned long interval, unsigned int repetitions);
  
  /**
   * \brief Stop a timer slot.
   * \param index The index of the slot to stop.
   * 
   * If you want to stop a timer slot you must call this method with the
   * index of the slot to stop. If index is not defined, the first slot is
   * stopped.
   * 
   * \see resume() restart()
   */
  void stop(byte index = 0);

  /**
   * \brief Stop all timer slot.
   * 
   * Used to stop all timer slots.
   * 
   * \see stop()
   */
  void stopAll();

  /**
	* \brief The timer's loop phase.
	* 
	* Must be called to ensure that the timer slots are working as intended.
	*/
  virtual void loop();

};

#endif //TTIMER_H
