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

double cur_val = 91.23;
double set_val = 100.01;

char curval_buffer[7];
char setval_buffer[7];
char value_disp[18];

void setup() {
  Serial.begin(115200);
}

void loop() {
  dtostrf(cur_val, 5, 2, curval_buffer);
  dtostrf(set_val, 5, 2, setval_buffer);

  sprintf(value_disp, "%s / %s", curval_buffer, setval_buffer);
  Serial.print("value : ");
  Serial.println(value_disp);

  cur_val += 0.327;
  delay(1000);
}

#endif
