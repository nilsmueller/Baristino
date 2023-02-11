#pragma once

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif


#include <OneWire.h>
#include <DallasTemperature.h>
#include "SSR.h"
#include "PID.h"

#include "../configuration.h"
#include "../statusTools.h"


namespace ThermoBlock {

enum class Status : uint8_t {
  SUCCESS,
  ERR_MINTEMP,
  ERR_MAXTEMP,
  ERR_HEATINGFAILED,
  ERR_TEMPSENSORCONNECTION,
  ERR_TEMPSENSORVALUE,
  ERR_TEMPSENSORTIMEOUT,
};

enum class TBState : uint8_t {
  OFF,
  HEATING,
  STEADYSTATE,
};

using IntResult = StatusResult<Status, uint16_t>;
using doubleResult = StatusResult<Status, double>;
using boolResult = StatusResult<Status, bool>;


// singleton class
class PIDHeater{
  public:
  PIDHeater(uint8_t pinSSR, uint8_t pinTempsensor);
  
  // General
  void initialize();
  void setTemperature(double setPoint);
  void update();

  // FSM 
  bool isOff();
  bool isHeating();
  bool isSteadyState();
  
  // PID Control
  void setPIDConstants(double Kp, double Ki, double Kd);
  void startPIDControl();
  void stopPIDControl();
  void watchPIDControl();
  double getPIDinput();
  double getPIDsetpoint();
  double getPIDouput();

  // non PID Control
  void powerOn();
  void powerOff();

  // Temperature Sensor
  double getTemperature();

  TBState getState();
  void power(double percentage);
  double getCurrentPower();

  private:

  // Pins
  int m_pinSSR;
  int m_pinTempsensor;

  // Temperature Sensor
  OneWire m_oneWire;
  DallasTemperature m_tempSensor;
  double m_currentTemperature;
  void setupTempSensor();
  void requestTemp();
  unsigned long m_lastTempRequest = millis();         // timestamp of the last temp sensor request in ms
  unsigned int m_tempSensorsDelay;   
  
  // PWM
  uint8_t m_pwmResolution = 6;
  double m_pwmCarrierFrequency = 50.0;


  // PID
  PID m_PID;
  SolidStateRelay m_SSR;
  double m_setTemperature = 20.0;
  double m_pidInput;
  double m_pidOutput;
  double m_pidSetpoint;
  double m_pidLastInputs[10] = {};
  double m_pidSetpointTolerance = 1.0;
  double m_Kp = 2.2;
  double m_Ki = 0.0;
  double m_Kd = 45.0;
  void updatePIDControl();
  void checkForSteadyState();

  // FSM
  TBState m_currentState = TBState::OFF;

};


}