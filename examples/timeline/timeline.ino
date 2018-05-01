//Required hardware: Push button, 1K Ohm resistor, LED, 330 Ohm resistor

//Required wiring:
//Pin D4 => 1K Ohm resistor => button leg 1
//GND => button leg 2
//Pin D3 => 330 Ohm => LED anode (LED+, long leg)
//LED cathode (LED-, short leg) => GND

#include <TDuino.h>

#define BUTTON_PIN 4
#define LED_PWM_PIN 3

void tlCallback(byte slotIdx, float progress); //Forward declaration

TPin led(LED_PWM_PIN, OUTPUT);
TButton button(BUTTON_PIN);
TTimeline tline(tlCallback, 2);
byte currentSlot = 0;

void tlCallback(byte slotIdx, float progress)
{
  //Convert the progress to a PWM value and set it to the LED
  if (slotIdx == 0) led.pwm(TL_MapToInt(progress, 0, 255)); //Fade in
  else led.pwm(TL_MapToInt(progress, 255, 0)); //Fade out, invert progress
}

void buttonPress(byte buttonPin, int state)
{
  //If any slots are active, we exit to avoid conflicts between slots
  if (tline.firstActive() >= 0) return;
  
  //Set current slot to start in 2 seconds with a duration of 500 millis
  tline.set(currentSlot, 500, 2000);
  
  //Invert bits - this will turn 0 to 1 or 1 to 0
  currentSlot ^= 1;
}

void setup()
{
  //Setup the LED and the button first
  led.setup();
  button.setup();

  //Attach callback to button
  button.onPress(buttonPress);
}

void loop()
{
  //Loop the button; led is not looped because
  //TPin::loop() does nothing
  button.loop();
  tline.loop();
}