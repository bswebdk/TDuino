//Required hardware: Push button, 1K Ohm resistor

//Required wiring:
//Pin D4 => 1K Ohm resistor => button leg 1
//GND => button leg 2

#include <TDuino.h>

#define BUTTON_PIN 4

TPin led(LED_BUILTIN, OUTPUT);
TButton button(BUTTON_PIN);

void buttonPress(byte buttonPin, int state)
{
  //Flip the state of the LED
  led.flip();
}

void buttonRelease(byte buttonPin, int state)
{
  //Turn off the LED
  led.off();
}

void setup()
{
  //Attach callbacks
  button.onPress(buttonPress);
  button.onRelease(buttonRelease);
  
  //Set the button's repeat rate. This will cause the LED
  //to blink in the specified rate (200ms) when the button
  //is held down for more than 500 milliseconds.
  button.setRepeat(500, 200);
  
  //Ensure the LED is off for starters
  led.off();
}

void loop()
{
  //Loop the button; led is not looped because
  //TPin::loop() does nothing
  button.loop();
}
