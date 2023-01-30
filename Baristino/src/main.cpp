#include <Arduino.h>
#include "configuration.h"
#include "Peripherals/RotaryEncoderInterrupt.h"

RotaryEncoderInterrupt encoder = RotaryEncoderInterrupt(pindef::BREWGROUP_ENCODER_INPUT_A, pindef::BREWGROUP_ENCODER_INPUT_B);

void setup() {
  Serial.begin(115200);
  encoder.initialize();
}


void loop() {
  //encoder.print();
  delay(10);
}

/*
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
  //myCoffeeMachine.updateMenuSelection();
}

*/


