/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TPin.h    
  
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

#ifndef TPIN_H
#define TPIN_H
#include "TBase.h"

/**
 * \brief Handles a I/O for a single pin.
 * 
 * TPin is used to handle different I/O tasks for a pin.
 */
class TPin : public TBase {

#ifdef TDUINO_DEBUG
private:
  bool isPinValid(const char *token = NULL);
#endif //TDUINO_DEBUG

protected:

  byte pin; ///< The pin assigned to this instance.
  byte mode; ///< The mode used for the pin (INPUT / INPUT_PULLUP / OUTPUT).
  bool analog; ///< True if the pin is detected as being an analog pin.

  /**
   * \brief Set the pin's mode.
   * 
   * If you want to change the pins mode to something else than what was
   * defined in the constructor, you can do it by calling setMode().
   * 
   * \see getMode() TPin()
   */
  void setMode(int mode);

public:

  /**
   * \brief Default constructor for the TPin class.
   * 
   * The constructor will set the pin and mode to invalid values. You MUST
   * call TPin::attach() before the instance of TPin is of any use.
   * 
   * \see attach()
   */
  TPin();
  
  /**
   * \brief The contructor for the pin class.
   * \param pin The pin to be used by this instance.
   * \param mode The mode to be used with the pin.
   * 
   * Constructs an instance of the TPin class and calls attach() in order to setup
   * the pin with the values provided.
   *
   * \see attach() 
   */
  TPin(byte pin, byte mode);
  
  /**
   * \brief Attach the instance of TPin to a pin.
   * \param pin The pin to which the instance should be attached.
   * \param mode The mode to be used for the pin.
   * 
   * If you are using the default constructor which takes no arguments
   * (eg. if creating an array of TPin), you MUST call attach() on each instance
   * of TPin in order for the pin to work.
   * 
   * attach() will store the values provided as arguments in #pin and #mode and test
   * wheter the pin is analog and store the value in #analog. When this is done, the
   * mode of the pin will be selected with pinMode().
   */
  virtual void attach(byte pin, byte mode = INPUT);
  
  /**
   * \brief Enable / disable a digital pin.
   * \param on Should be LOW (disable) or HIGH (enable)
   * 
   * Uses digitalWrite to set the pins state.
   * 
   * \see flip() on() off()
   */
  void enable(byte on);
  
  /**
	* \brief Flip pin state.
	* 
	* This will flip / invert the state of the pin. If the pint is LOW it
	* will be set to HIGH vice versa. Uses digitalWrite regardles of the
	* pin being detected as analog.
   * 
   * \see on() off() enable()
	*/
  void flip();
  
  /**
	* \brief Turn off the pin.
	* 
	* Sets the pin state to LOW.

   * \see flip() on() enable()
*/
  void off();

  /**
	* \brief Turn on the pin.
	* 
	* Sets the pin state to HIGH.
   * 
   * \see flip() off() enable()
	*/
  void on();

  /**
	* \brief Set the pin to a PWM value.
	* \param value The PWM value to be set.
	* 
	* Sets the pin's PWM value to the value given. The PWM value is set using
   * analogWrite. To reduce overhead there is being performed no check of the
   * pins PWM capabilities (unless compiled in debug mode). You should do this
   * with hasPwm()
	* 
	*/
  void pwm(byte value);

  /**
   * \brief Read the state of the pin.
   * \returns The state of the pin.
   * 
   * This will return the state retreived with analogRead for analog pins and
   * digitalRead for digital pins.
   * 
   * \see state()
   */
  int read();

  /**
   * \brief Get the digital state of the pin.
   * \returns The state of the pin.
   * 
   * This will return the state retreived with digitalRead even if the pin is
   * analog.
   * 
   * \see read()
   */
  int state();

  /**
	* \brief Checks if the pin is analog.
	* \returns True if the pin is analog.
	* 
	* Returns the value of #analog.
	*/
  bool isAnalog();
  
  /**
	* \brief Checkt if the pin has PWM capabilities.
	* \returns True if the pin has PWM capabilities.
	* 
	* You can use this method to detect is the pin has PWM capabilities. This
   * method must return true if the pwm() method should work with the pin.
   * 
	* \see pwm().
	*/
  bool hasPwm();

  /**
	* \brief Get the pin's mode.
	* \returns The pin mode.
	* 
	* Used to retreive the mode used by the pin. The return value is equal to
  * the mode set in the constructor or with setMode().
	* \see TPin().
	*/
  int getMode();

  /**
	* \brief Get the pin's number.
	* \returns The pin number.
	* 
	* Used to retreive the pin number used by this instance of TPin.
	* The value return is equal to the pin number set in the constructor.
	* \see TPin().
	*/
  int getPin();

};

#endif //TPIN_H
