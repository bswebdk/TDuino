//Required hardware: LED diode, 330 Ohm resistor

//Required wiring:
//Pin D3 => 330 Ohm => LED anode (LED+, long leg)
//LED cathode (LED-, short leg) => GND

#include <TDuino.h>

#define NUM_LEDS 2
#define LED_PWM_PIN 3

const byte LED_PINS[NUM_LEDS] = { LED_BUILTIN, LED_PWM_PIN };

TPinOutput leds[NUM_LEDS];

void setup()
{
   //Attach the LED's to pins
   for (byte i = 0; i < NUM_LEDS; i++) leds[i].attach(LED_PINS[i], OUTPUT);
   
   //Start pulsing the builtin LED
   leds[0].pulse(500);
   
   //You could create a 10 repetitions of a pulse pattern which is off for
   //100 milliseconds and on for 500 milliseconds with:
   //led1.pulse(100, 500, 10, LOW);
   
   //Start oscillating the LED on the PWM pin
   leds[1].oscillate(500);
   
   //You could create a 10 repetitions of a oscillating pattern which is
   //rising for 100 milliseconds and falling for 500 milliseconds with:
   //led[1].oscillate(100, 500, 10, 0);
   
   //By default the oscillation goes from 0 to 255, but if you want to
   //limit the oscillating to be between 100 and 200, you could:
   //led[1].oscillate(100, 500, 10, 0, 100, 200);
}

void loop()
{
   //Loop the LED's
   for (byte i = 0; i < NUM_LEDS; i++) leds[i].loop();
}
