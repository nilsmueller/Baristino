#include <Arduino.h>

#define DEBUG false



#if not DEBUG
#include "CoffeeMachine.h"
CoffeeMachine myCoffeeMachine = CoffeeMachine();

void setup() {
  // STBY Button
  pinMode(pindef::STDBY_BTN_IN, INPUT);
  pinMode(pindef::STDBY_BTN_VCC, OUTPUT);
  digitalWrite(pindef::STDBY_BTN_VCC, HIGH);
  
  Serial.begin(115200);
  delay(1000);

  Serial.println("Switch On Power Now!");
  delay(3000);

  myCoffeeMachine.initialize();
  myCoffeeMachine.warmup();


}

int button_state;

void loop() { 

  button_state = digitalRead(pindef::STDBY_BTN_IN);
  myCoffeeMachine.updateSensors();

  Serial.print("BT: ");
  Serial.print(button_state);
  myCoffeeMachine.printSensorValues();
  myCoffeeMachine.updateLCD();
}


#endif






#if DEBUG
#include "Utils/debug.h"

// set debug mode
DEBUGMODE mode = DEBUGMODE::ONOFFBUTTON_01;


void setup() {
  Serial.begin(115200);
  delay(1000);

  debug(mode, true);
}

void loop() {
  debug(mode, false);
}

#endif