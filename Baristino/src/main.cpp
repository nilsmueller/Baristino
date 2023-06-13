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
  myCoffeeMachine.printStatus();
}


#endif






#if DEBUG


#include "configuration.h"
//#include "Peripherals/BrewGroup.h"
#include "Peripherals/Grinder.h"
//BrewGroup::Ensemble m_unitBrewGroup = BrewGroup::Ensemble(pindef::BREWGROUP_MOTOR_IN_1, pindef::BREWGROUP_MOTOR_IN_2, pindef::BREWGROUP_MOTOR_ENA, pindef::BREWGROUP_ENCODER_INPUT_A, pindef::BREWGROUP_ENCODER_INPUT_B, pindef::BREWGROUP_MOTOR_AMPMETER);
Grinder::Hopper m_unitGrinder = Grinder::Hopper(config::STEPPER_STEPS_PER_REV, pindef::STEPPER_1, pindef::STEPPER_2, pindef::STEPPER_3, pindef::STEPPER_4, pindef::HOPPER_LOADCELL_DOUT, pindef::HOPPER_LOADCELL_SCK, pindef::SSR_GRINDER);

void setup() {
  Serial.begin(115200);
  delay(1000);

  //LCD::initialize();
  m_unitGrinder.initialize();

  delay(1000);
  m_unitGrinder.homeFlap();
  m_unitGrinder.tareScale();
}


void loop() {
  delay(50);
  m_unitGrinder.updateScale();
  Serial.println(m_unitGrinder.getCurrentAmount());

/*
  myCoffeeMachine.updateSensors();
  myCoffeeMachine.printSensorValues();
  myCoffeeMachine.updateLCD();
  myCoffeeMachine.printStatus();
  */


}

#endif