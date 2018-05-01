/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TButton.h 
  
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

#ifndef TBUTTON_H
#define TBUTTON_H
#include "TPinInput.h"

/**
 * \brief Handles simple push buttons.
 * 
 * This class is used to attach a callback event to a pin which has a push buttons
 * attached to it.
*/
class TButton : protected TPinInput {

private:
  unsigned int delay1, delay2;
  unsigned long lastRepeat;

/// \cond HIDDEN_FIELD
protected:
  virtual void falling();
/// \endcond
  
public:
  /**
	* \brief Constructs an instance of TButton.
	* 
	* \param pin The pin on which the push button is attached.
	* 
	* The pin to be used with the button will be setup as INPUT_PULLUP.
	*/
  TButton(byte pin);
  
  /**
	* \brief Detects if the button is pressed / down.
	* 
	* \return True is the button is pressed / down.
	*/
  bool isPressed();
  
  /**
	* \brief Sets the repeat rate for the button.
	* 
	* \param firstDelay Delay (in milliseconds) before the first repeat.
	* \param followingDelay Delay used for sub-sequent repeats.
	* 
	* If both arguments are set to zero (the default), no repeats will be
	* generated.
	*/
  void setRepeat(unsigned int firstDelay, unsigned int followingDelay);

  /**
	* \brief Set the callback used for presses.
	* 
	* \param callback The callback to be used for presses.
	* 
	* The callback method is invoked each time the button is pressed (the pin goes to LOW).
	* If repeat is enabled with setRepeat() this is the callback which will be called
	* for each repetition. The argument passed to the callback method is the pin number
	* on which the button is attached and that pins state.
	*/
  void onPress(TPinInputCallback callback);
  
  /**
	* \brief Set the callback used for releases.
	* 
	* \param callback The callback to be used for releases.
	* 
	* The callback method is invoked each time the button is released (the pin goes to HIGH).
	* 
	* \see onPress()
	*/
  void onRelease(TPinInputCallback callback);

  /**
	* \brief Setup the button.
	* 
	* Setup will initialize debounce to 10 milliseconds if debounce is zero (default). If
	* you want to disable debounce, you must call setDebounce() and pass a zero to it after
   * you have called setup().
	*/
  virtual void setup();
  
  /**
	* \brief The buttons loop phase.
	*/
  virtual void loop();
  
  /**
	* \brief Get the debounce for the button.
	* \see TPinInput::getDebounce()
	*/
  using TPinInput::getDebounce;
  
  /**
	* \brief Set the debounce for the button.
	* \see TPinInput::setDebounce()
	*/
  using TPinInput::setDebounce;
  
};

#endif
