#include <Arduino.h>


#include "CoffeeMachine.h"
CoffeeMachine myCoffeeMachine = CoffeeMachine();

void setup() {
  Serial.begin(115200);
  delay(1000);
  LCD::initialize();
  Serial.println("Switch On Power Now!");
  delay(5000);

  Serial.println("Initializing");
  myCoffeeMachine.initialize();
  delay(1000);

  Serial.println("Starting Warm Up Phase");
  myCoffeeMachine.warmup();
  //myCoffeeMachine.makeCoffee();

}


void loop() { 
  //myCoffeeMachine.update();
}





/*
// THIS WORKS!

#include "configuration.h"
#include "Peripherals/Pump.h"

WaterControl::Pump myPump = WaterControl::Pump(pindef::SSR_WATERPUMP, pindef::BEVERAGE_LOADCELL_SCK, pindef::BEVERAGE_LOADCELL_DOUT);


void setup() {
  Serial.begin(115200);
  delay(1000);
  myPump.initialize();
  delay(1000);
  myPump.pumpAmount(50);
}


void loop() {
  //myPump.updateScale();

  //double amount  = myPump.getCurrentAmount();
  //Serial.println(amount);
  //delay(100);
}
*/

