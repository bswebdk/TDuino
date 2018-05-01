//Required hardware: LED diode, 330 Ohm resistor

//Required wiring:
//Pin D3 => 330 Ohm => LED anode (LED+, long leg)
//LED cathode (LED-, short leg) => GND

#include <TDuino.h>

#define LED_PWM_PIN 3

void timerCallback(byte timerIndex); //Forward declaration

TPin led1(LED_BUILTIN, OUTPUT);
TPin led2(LED_PWM_PIN, OUTPUT);
TTimer timer(timerCallback, 3); //Three timer slots

void timerCallback(byte timerIndex)
{
  switch (timerIndex)
  {
    case(0): //First timer slot
      led1.flip();
      break;
    
    case(1): //Second timer slot
      led2.flip();
      break;
    
    case(2): //Third timer slot
      timer.restart(1); //Restart second timer slot which has been finished for ~2 seconds.
      break;
  }
}

void setup()
{  
   //Setup pins and timer first
   led1.setup();
   led2.setup();
   timer.setup();
   
   //Set first timer to 250ms delay and 0 (indefinite) repetitions
   timer.set(0, 250, 0); 
   
   //Set second timer to 500ms delay and 10 repetitions (5 seconds show)
   timer.set(1, 500, 10);
   
   //Set third timer to 7 seconds delay and 0 (indefinite) repetitions
   timer.set(2, 7000, 0);
}

void loop()
{
   //led1 & led2 are not looped since TPin::loop() does nothing
   //led1.loop();
   //led2.loop();
   
   //Loop the timer
   timer.loop();
}
