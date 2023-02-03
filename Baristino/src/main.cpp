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
}




/*
#include "configuration.h"
#include "Peripherals/BrewGroup.h"

BrewGroup::Ensemble myBG = BrewGroup::Ensemble(pindef::BREWGROUP_MOTOR_IN_1, pindef::BREWGROUP_MOTOR_IN_2, pindef::BREWGROUP_MOTOR_ENA, pindef::BREWGROUP_ENCODER_INPUT_A, pindef::BREWGROUP_ENCODER_INPUT_B, pindef::BREWGROUP_MOTOR_AMPMETER);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Switch On Power Now!");
  delay(3000);

  myBG.initialize();
  delay(1000);

  myBG.moveUp();
  while (myBG.isMovingUp()) {
    Serial.print("AMPS : ");
    Serial.print(myBG.getCurrent());
    Serial.print("    Position : ");
    Serial.print(myBG.getPosition());
    Serial.print("   isStalled : ");
    Serial.print(myBG.isStalled());
    Serial.print("   isMovingUp : ");
    Serial.print(myBG.isMovingUp());
    Serial.print("   isCurrentNominal : ");
    Serial.print(myBG.isCurrentNominal());
    Serial.println();

    myBG.checkIfHomed();
  }
}

void loop() {
  Serial.print("AMPS : ");
  Serial.println(myBG.getCurrent());
  delay(1000);
}

*/
