/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TPinInput.h    
  
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

#ifndef TPININPUT_H
#define TPININPUT_H
#include "TPin.h"

/**
 * \brief The callback type used for input pins
 */
typedef void (*TPinInputCallback)(byte, int);

/**
 * \brief Attach events to handle input from a pin.
 * 
 * TPinInput is used to monitor changes on a digital or analog input pin and
 * whenever a change is registred on the specific pin, an event is triggered.
 */
class TPinInput : public TPin {

private:

  byte sampleIdx;
  int dummy;
  unsigned int sampleVal;
  
protected:

  TPinInputCallback callbacks[2]; ///< Callbacks for onFalling and onRising
  byte deviation; ///< The amount of deviation required to trigger an event
  byte samples; ///< The number of samples to use for events
  int* sampleBuffer; ///< Buffer used for storing multiple samples
  int lastState; ///< The last known state of the pin. See loop().
  unsigned int debounce; ///< The amount of debounce used with the pin.
  unsigned long changeMillis; ///< The time of the last state change

  /**
   * \brief Method used to reset all class internal variables.
   * 
   * This method is called by various constructors when all the private and
   * protected values are to be reset to their default values. You can override
   * this method if you need to change any default value of TPinInput.
   */
  virtual void defaults();
  
  /**
	* \brief Method called when the pins state is falling.
	* 
	* This method is called whenever the pin state is detected as being lower than
   * the last time (minding debounce and deviation). You can override this method
   * in order to change the functionality a falling pin state. By default this
   * method calls the event handler stored in #callbacks [0].
	*/
  virtual void falling();
  
  /**
	* \brief Method called when the pins state is rising.
	* 
	* This has inverted functionality of falling() and by default it does nothing
   * but calling callbacks [1].
	*/
  virtual void rising();
  
public:

  /**
   * \brief The default contructor for the TPinInput class.
   * 
   * Please see docs for the default constructor of TPin.
   * 
   * \see TPin::TPin() attach()
   */
  TPinInput();
  
  /**
   * \brief The contructor for the pin class.
   * \param pin The pin to be used by this instance.
   * \param mode The mode to be used with the pin.
   * 
   * \see TPin::TPin(byte, byte) for further info.
   * \see attach() for valid values.
   */
  TPinInput(byte pin, byte mode = INPUT);
  
  /**
	* \brief The destructor for the class.
	* 
	* Releases any memory used for sample buffer.
	*/
  virtual ~TPinInput();
  
  /**
   * \brief Attach the instance of TPinInput to a pin.
   * \param pin See TPin::attach()
   * \param mode See TPin::attach()
   * 
   * Valid values for mode are INPUT and INPUT_PULLUP - if any other value
   * if provided, INPUT is used.
   * 
   * \see TPin::attach()
   */
  virtual void attach(byte pin, byte mode = INPUT);
  
  /**
   * \brief Get the average value of multiple readings from the pin.
   * \param samples The number of samples to get (MUST be above 2).
   * \returns The average value of the pin.
   * 
   * This will return the average value of _samples_ readings from the pin with
   * analogRead for analog pins and digitalRead for digital pins. If any debounce
   * has been specified, the debounce will cause a (blocking!) delay of _debounce_
   * length before each read.
   * 
   * \see setDebounce()
   */
  int read(byte samples);
  
  using TPin::read; //Required for overload

  /**
	* \brief Get the amount of debounce used with events.
   *
	* \see setDebounce() #debounce.
	*/
  unsigned int getDebounce();
  
  /**
	* \brief Sets the debounce value.
	* \param debounce The debounce value to be used.
	* 
	* Sets the debounce value to be used when detecting changes in the pins state.
   * The value is defined in milliseconds and is used as the minimum amount of time
   * there has to elapse between state changes.
   * 
   * Please not that the specified debounce is only used for events and by the
   * read(byte) function. All other methods used for reading the pin does not mind
   * the debounce.
	* 
	* \see getDebounce() #debounce
	*/
  void setDebounce(unsigned int debounce);
  
  /**
   * \brief Get the amount of deviation used with events.
   * 
   * \see setDeviation()
   */
  byte getDeviation();

  /**
  * \brief Set the amount of deviation used to detect state change.
  * 
  * By default any change in pin state will fire an event (deviation = 0), but since 
  * readings from analog pins may fluctuate a bit you may be flooded with useless events.
  * By setting deviation, events will only be triggered when the pin state differs by the
  * amount of deviation specified.
  * 
  * __Known "issue":__ When deviation is used, some values will never be registered for
  * the pin. If deviation is set to 5 and the initial state of the pin is read as 4,
  * you will never get a value of 0 from the pin which may be an issue for some
  * applications.
  * 
  * \see onRising() onFalling()
  */
  void setDeviation(byte deviation);
  
  /**
   * \brief Get buffered value of the sample buffer.
   * \param sampleIndex Index of sample to get.
   * \returns The buffered value from the sample buffer or -1 if buffering is not used.
   * 
   * If _sampleIndex_ is zero (default value), the average of the samples in the buffer is returned.
   * If _sampleIndex_ is non-zero, the single sample at the index (1..SampleBufferSize) is returned.
   * 
   * \see setSamples()
   */
  int getBufferedValue(byte sampleIndex = 0);

  /**
   * \brief Get the number of samples used with events.
   * 
   * \see setSamples()
   */
  byte getSamples();

  /**
  * \brief Set number of samples (reads) used with events.
  * \param samples The number of samples to use (2..60).
  * \param buffered If true the samples will be buffered.
  * 
  * By default the pin is read once and if the state differs (minding any deviation) from
  * the previously read state, an event is triggered. In order to increase accuracy and
  * reduce electrical fluctuations, you can specify how many samples (reads) should be
  * retreived in order to calculate an average value which will increase accuracy.
  * 
  * If samples is not in the valid range 2..60, sampling will be disabled! The low limit
  * is obvious, the high level is set because the state is stored as an unsigned int with
  * a max value of 65535 (1023*60 = 61380).
  * 
  * If _buffered_ is false, the pin will be read _samples_ times in order to calculate
  * an average which is then used to trigger an event. This means that whenever an event
  * is triggered, the value passed to it will be calculated from _samples_ new readings.
  * 
  * If _buffered_ is true, _samples_ readings from the pin will be stored in a buffer
  * (BEWARE: dynamic memory usage!) in order to calculate an average reading for the
  * event. This means that the value passed to the event is an average of one new
  * reading combined with _samples_-1 old readings.
  * 
  * If any debounce is defined for the pin, the debounce will be the minimum amount
  * of time there has to elapse between each sample. For non-buffered sampling, the
  * event rate will be approximately <b>pin_read_time + (debounce * samples)</b>, for
  * buffered sampling the rate will be <b>pin_read_time + debounce</b>
  * 
  * \see read(byte) onRising() onFalling() setDebounce()
  */
  void setSamples(byte samples, bool buffered = false);

  /**
	* \brief The pin's loop method.
	* 
	* Must be called in order to read an calculate pin state as requested. If loop() is
   * not called, no events will be triggered by the pin.
	* 
	* \see onRising() onFalling() rising() falling() TPin::loop()
	*/
  virtual void loop();

  /**
	* \brief Assign a callback to handle falling pin state.
	* \param callback The callback to be used.
	* 
	* Assigns the argument given as being the callback used whenever the pin's state
   * is registred as falling (lower than last poll). You can clear a previously set
   * callback by using NULL as argument.
	* 
	* The arguments passed to the callback are the pin number and the new state which
   * has been detected on the pin.
	* 
	* \see onRising() setSamples() setDeviation() falling() rising().
	* 
	* \code
	* void pinStateChange(byte pin, int newState)
	* {
	*   //React to the new pin state
	* }
	* 
	* TPinInput pinInput(pinNumber);
	* pinInput.onFalling(pinStateChange);
	* \endcode
	*/
  void onFalling(TPinInputCallback callback);//void (*callback)(byte, int));
  
  /**
	* \brief Assign a callback for rising pin state.
	* \param callback The callback to be used.
	* 
	* Is the same as onFalling() but for a rising pin state.
	* 
	* \see onFalling()
	*/
  void onRising(TPinInputCallback callback);//void (*callback)(byte, int));

};

#endif //TPININPUT_H
