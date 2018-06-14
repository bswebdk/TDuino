//Required hardware: LED diode, 330 Ohm resistor

//Required wiring:
//Pin D3 => 330 Ohm => LED anode (LED+, long leg)
//LED cathode (LED-, short leg) => GND

#include <TDuino.h>

#define NUM_LEDS 2
#define LED_PWM_PIN 3

const byte LED_PINS[NUM_LEDS] = { LED_BUILTIN, LED_PWM_PIN };

void timerCallback(byte timerIndex); //Prototype, fully declared later

TPin leds[NUM_LEDS];
TTimer timer(timerCallback, 3); //Three timer slots


void timerCallback(byte timerIndex)
{
  switch (timerIndex)
  {
    case(0): //First timer slot
      leds[0].flip();
      break;
    
    case(1): //Second timer slot
      leds[1].flip();
      break;
    
    case(2): //Third timer slot
      timer.restart(1); //Restart second timer slot which has been finished for ~2 seconds.
      break;
  }
}

void setup()
{
   //Attach LED's to pins
   for (byte i = 0; i < NUM_LEDS; i++) leds[i].attach(LED_PINS[i], OUTPUT);
   
   //Set first timer to 250ms delay and 0 (indefinite) repetitions
   timer.set(0, 250, 0); 
   
   //Set second timer to 500ms delay and 10 repetitions (5 seconds show)
   timer.set(1, 500, 10);
   
   //Set third timer to 7 seconds delay and 0 (indefinite) repetitions
   timer.set(2, 7000, 0);
}

void loop()
{
   //LED's are not looped since TPin::loop() does nothing
   //for (byte i = 0; i < NUM_LEDS; i++) leds[i].loop();
   
   //Loop the timer
   timer.loop();
}
