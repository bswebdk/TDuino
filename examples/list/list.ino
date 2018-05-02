//Required hardware: LED diode, 330 Ohm resistor

//Required wiring:
//Pin D3 => 330 Ohm => LED anode (LED+, long leg)
//LED cathode (LED-, short leg) => GND

#include <TDuino.h>

#define LED_PWM_PIN 3

TPinOutput led1(LED_BUILTIN);
TPinOutput led2(LED_PWM_PIN);
TList list(2);

void setup()
{
   //Add objects to list
   list.set(0, &led1);
   list.set(1, &led2);
   
   //Setup all objects in the list
   list.setup();
   
   //Start pulsing the builtin LED
   led1.pulse(500);
   
   //Start oscillating the LED on pin 3
   led2.oscillate(500);
}

void loop()
{
   //Loop all objects in the list
   list.loop();
}
