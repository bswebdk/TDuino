//Required hardware: LED diode, 330 Ohm resistor

//Required wiring:
//Pin D3 => 330 Ohm => LED anode (LED+, long leg)
//LED cathode (LED-, short leg) => GND

#include <TDuino.h>

#define LED_PWM_PIN 3

TPinOutput led1(LED_BUILTIN);
TPinOutput led2(LED_PWM_PIN);

void setup()
{  
   //Start pulsing the builtin LED
   led1.pulse(500);
   
   //You could create a 10 repetitions of a pulse pattern which is off for
   //100 milliseconds and on for 500 milliseconds with:
   //led1.pulse(100, 500, 10, LOW);
   
   //Start oscillating the LED on pin 3
   led2.oscillate(500);
   
   //You could create a 10 repetitions of a oscillating pattern which is
   //rising for 100 milliseconds and falling for 500 milliseconds with:
   //led2.oscillate(100, 500, 10, 0);
   
   //By default the oscillation goes from 0 to 255, but if you want to
   //limit the oscillating to be between 100 and 200, you could:
   //led2.oscillate(100, 500, 10, 0, 100, 200);
}

void loop()
{
   //Loop the LED's
   led1.loop();
   led2.loop();
}
