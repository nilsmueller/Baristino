#include <Arduino.h>

#include "CoffeeMachine.h"
CoffeeMachine myCoffeeMachine = CoffeeMachine();

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Switch On Power Now!");
  delay(3000);

  //Serial.println("Initializing");
  myCoffeeMachine.initialize();
  //delay(1000);

  //Serial.println("Starting Warm Up Phase");
  myCoffeeMachine.warmup();
  //myCoffeeMachine.fillSystem();

}




void loop() { 
  myCoffeeMachine.updateSensors();
  myCoffeeMachine.printSensorValues();
  myCoffeeMachine.updateLCD();
  myCoffeeMachine.updateMenuSelection();
}

