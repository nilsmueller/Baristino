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
#include "Peripherals/Grinder.h"

Grinder::Hopper m_unitGrinder = Grinder::Hopper(config::STEPPER_STEPS_PER_REV, pindef::STEPPER_1, pindef::STEPPER_2, pindef::STEPPER_3, pindef::STEPPER_4, pindef::HOPPER_LOADCELL_DOUT, pindef::HOPPER_LOADCELL_SCK, pindef::SSR_GRINDER);

double res = 0.0f;

void setup() {
Serial.begin(115200);
delay(1000);
m_unitGrinder.initialize();
m_unitGrinder.tareScale();

}

void loop() {
  m_unitGrinder.updateScale();
  res = m_unitGrinder.getCurrentAmount();
  Serial.println(res);
  delay(100);

}

#endif
