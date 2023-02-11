#include <Arduino.h>

#define DEBUG false


#if not DEBUG
#include "CoffeeMachine.h"
CoffeeMachine myCoffeeMachine = CoffeeMachine();

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Switch On Power Now!");
  delay(3000);

  myCoffeeMachine.initialize();
  myCoffeeMachine.warmup();

}

void loop() { 
  myCoffeeMachine.updateSensors();
  myCoffeeMachine.printSensorValues();
  myCoffeeMachine.updateLCD();
}
#endif

#if DEBUG
#include "configuration.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(pindef::DRAIN_VALVE, OUTPUT);

  Serial.println("Switch power on");
  delay(3000);

  Serial.println("Open Valve:");
  digitalWrite(pindef::DRAIN_VALVE, HIGH);
  delay(1000);

  Serial.println("Close Valve:");
  digitalWrite(pindef::DRAIN_VALVE, LOW);
}

void loop() {
}

#endif
