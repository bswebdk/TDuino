//Required hardware: none!
//Required wiring: none!

#include <TDuino.h>

//Use the builtin LED as output
TPin pin(LED_BUILTIN, OUTPUT);

//delay() is used in this example for simplification. Normally
//you would never use delay() unless there are no other options!

void method1()
{
  for (byte i = 0; i < 2; i++)
  {
    //This will flip/invert pin state from on to off or vice versa
    pin.flip();

    //Wait ½ a second
    delay(500);
  }
}

void method2()
{
  //Turn the pin on
  pin.on();
  
  //Wait ½ a second
  delay(500);
  
  //Turn the pin off
  pin.off();
  
  //Wait ½ a second
  delay(500);
}

void method3()
{
  //Enable the pin (turn it on)
  pin.enable(HIGH);

  //Wait ½ a second
  delay(500);
  
  //Disable the pin (turn it off)
  pin.enable(LOW);

  //Wait ½ a second
  delay(500);
}

void setup()
{
   //Setup pin first
   pin.setup();
   
   //Turn the pin off for starters
   pin.off();
}

void loop()
{
   //Use all three methods to blink the built in LED
   method1();
   method2();
   method3();
}