/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TPinOutput.h   
  
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

#ifndef TPINOUTPUT_H
#define TPINOUTPUT_H
#include "TPin.h"

/**
 * \brief Output functionality for a pin.
 * 
 * Extends the basic pin features with additional functionality like pulsing
 * and oscillating.
*/
class TPinOutput : public TPin {
  
private:

  int stateCur;
  byte task, stateInit, stateLow, stateHigh;
  unsigned long lastMillis;
  unsigned int msLow, msHigh, repeats, count;

  void counter();

protected:
/// \cond HIDDEN_FIELD
  virtual void defaults();
/// \endcond

public:

  /**
   * \brief Default constructor for TPinOutput.
   *
   * Please see docs for the default constructor of TPin.
   * 
   * \see TPin::TPin() attach()
   */
  TPinOutput();
  
  /**
   * \brief Attach the instance of TPinOutput to a pin.
   * \param pin See TPin::attach()
   * \param mode See TPin::attach()
   * 
   * The value of mode is ignored, always set to OUTPUT.
   * 
   * \see TPin()::attach()
   */
  virtual void attach(byte pin, byte mode = INPUT);
  
  /**
	* \fn oscillate(unsigned int intervalRising, unsigned int intervalFalling, unsigned int repetitions, byte initialState, byte stateLowest, byte stateHighest)
	* 
	* \brief Oscillates a pin using PWM.
	* 
	* \param intervalRising Milliseconds from low to high.
	* \param intervalFalling Milliseconds from high to low.
	* \param repetitions Number of repetitions to perform (default = 0).
	* \param initialState The initial PWM value to start from (default = 0).
	* \param stateLowest The lowest PWM value to use (default = 0).
	* \param stateHighest The highest PWM value to use (default = 255).
	* 
	* This method will oscillate a PWM signal between the lowest value defined by
	* _stateLowest_ and the highest value defined by _stateHighest_. The amount
	* of milliseconds it takes from lowest to highest is defined with _intervalRising_ and
	* _intervalFalling_ defines the equivalent for high to low. The oscillation will start
	* at _initialState_ and it will keep oscillating until _repetitions_
	* oscillations has been performed. If _repetitions_ is zero it will oscillate forever.
   * 
   * The overloaded versions of this method will use the default values described above to
   * define the omitted parameters.
	* 
	* __NOTE:__ If _initialState_ is not equal to _stateLowest_ or _stateHighest_, it
	* will be mapped to whichever of the two values is the nearest.
	*/
  void oscillate(unsigned int intervalRising, unsigned int intervalFalling, unsigned int repetitions, byte initialState, byte stateLowest, byte stateHighest);
  
  /**
	* \overload oscillate(unsigned int intervalRising, unsigned int intervalFalling, unsigned int repetitions, byte initialState)
	* \see oscillate()
	*/
  void oscillate(unsigned int intervalRising, unsigned int intervalFalling, unsigned int repetitions, byte initialState);

  /**
	* \overload oscillate(unsigned int interval, unsigned int repetitions, byte initialState)
   * 
   * _interval_ will be used for both _intervalRising_ and _intervalFalling_.
   * 
	* \see oscillate()
	*/
  void oscillate(unsigned int interval, unsigned int repetitions, byte initialState);

  /**
	* \overload oscillate(unsigned int interval, unsigned int repetitions)
   * 
   * _interval_ will be used for both _intervalRising_ and _intervalFalling_.
   * 
	* \see oscillate()
	*/
  void oscillate(unsigned int interval, unsigned int repetitions);

  /**
	* \overload oscillate(unsigned int interval)
   * 
   * _interval_ will be used for both _intervalRising_ and _intervalFalling_.
   * 
	* \see oscillate()
	*/
  void oscillate(unsigned int interval);
  
  /**
	* \fn pulse(unsigned int intervalLow, unsigned int intervalHigh, unsigned int repetitions, byte initialState)
	* 
	* \brief Pulses a pin between by switching it between LOW and HIGH.
	* 
	* \param intervalLow Milliseconds for the pin to be LOW.
	* \param intervalHigh Milliseconds for the pin to be HIGH.
	* \param repetitions Number of pulses to perform (default = 0).
	* \param initialState The initial state of the pulse (default = LOW).
	* 
	* This will start a pulse on the pin which will be LOW for _intervalLow_ milliseconds
   * and then HIGH for _intervalHigh_ milliseconds and so on. the pulse will start as
   * _initialState_ and it will continue until _repetition_ pulses have been performed.
   * If _repetitions_ is zero the pulse will go on forever.
   * 
   * The overloaded versions of this method will use the default values described above to
   * define the omitted parameters.
	*/
  void pulse(unsigned int intervalLow, unsigned int intervalHigh, unsigned int repetitions, byte initialState);
  
  /**
	* \overload pulse(unsigned int interval, unsigned int repetitions, byte initialState)
	* 
	* _interval_ will be used as both _intervalLow_ and _intervalHigh_
	* 
	* \see pulse()
	*/
  void pulse(unsigned int interval, unsigned int repetitions, byte initialState);
  
  /**
	* \overload pulse(unsigned int interval, unsigned int repetitions)
	* 
   * _interval_ will be used as both _intervalLow_ and _intervalHigh_
	* 
	* \see pulse()
	*/
  void pulse(unsigned int interval, unsigned int repetitions);
  
  /**
	* \overload pulse(unsigned int interval)
	* 
   * _interval_ will be used as both _intervalLow_ and _intervalHigh_
	* 
	* \see pulse()
	*/
  void pulse(unsigned int interval);
  
  /**
	* \brief The extended pins loop phase.
	* 
	* You must ensure that this method is called regularly or else any active
	* task will not be performed.
	*/
  virtual void loop();
  
  /**
	* \brief Stop the current pin task.
	* 
	* If you want to abort / stop an active oscillate or pulse you must call
	* this method.
	*/
  void stop();

};

#endif //TPINOUTPUT_H
