//Required hardware: LED diode, 330 Ohm resistor, 10K Ohm potentiometer

//Required wiring:
//Pin D3 => 330 Ohm => LED anode (LED+, long leg)
//LED cathode (LED-, short leg) => GND
//+5V => potentiometer leg 1
//GND => potentiometer leg 3
//A0 (Analog input 0) => potentiometer leg 2 (middle)

#include <TDuino.h>

#define LED_PWM_PIN 3
#define ANALOG_INPUT_PIN A0

TPinOutput led;
TPinInput pot;

void potCallback(byte pin, int state)
{
  //Set the LED to a mapped value of the potentiometer value
  //This will basically adjust the light intensity of the LED
  //to the reading of the potentiometer.
  led.pwm(map(state, 0, 1023, 0, 255));
}

void setup()
{
  //Attach the LED to pin 
  led.attach(LED_PWM_PIN);
  
  //Attach the pot to pin
  pot.attach(ANALOG_INPUT_PIN, INPUT);
   
  //Attach the same callback to both rising and falling state
  //of the potentiometer. You could use two different callbacks
  //(one for rise and one for fall) if you wanted to :)
  pot.onFalling(potCallback);
  pot.onRising(potCallback);
  
  //If you wanted to debounce (put some delay between readings)
  //the pot with 50ms, you could:
  //pot.setDebounce(50);
  
  //If you only wanted values which deviate with at least 5 from
  //the previous reading, you could:
  //pot.setDeviation(5);
}

void loop()
{
  //Loop the pins
  led.loop();
  pot.loop();
}
