#include "ThermoBlock.h"

// TODO remove the dependancy on "configuration.h". that should all be managed in CoffeeMachine.h

namespace ThermoBlock {


PIDHeater::PIDHeater(uint8_t pinSSR, uint8_t pinTempsensor) : 
m_oneWire(pinTempsensor),
m_tempSensor(&m_oneWire),
m_PID(&m_pidInput, &m_pidOutput, &m_pidSetpoint, true),
m_SSR(pinSSR, m_pwmResolution, m_pwmCarrierFrequency)
{
}



/**
Initialize the sensors and the Solid State Relay. Because of reasons i have not investigated
further yet, the TimerOne libraries requieres to be initialized in the setup loop of the arduino.
Therfore this task cannot be done at compile time.
*/
void PIDHeater::initialize() {
  m_PID.setTunings(m_Kp, m_Ki, m_Kd);
  m_PID.setSampleTimeMS(m_SSR.getPWMPeriod() / 1000);
  m_PID.setOutputLimits(0.0, 255.0);

  setupTempSensor();

  m_SSR.initialize();
  //powerOff();
}


bool PIDHeater::isOff() {return m_currentState == TBState::OFF;}
bool PIDHeater::isHeating() {return m_currentState == TBState::HEATING;}
bool PIDHeater::isSteadyState() {return m_currentState == TBState::STEADYSTATE;}

TBState PIDHeater::getState() {return m_currentState;}


/**
Setup and connect to the DS18B20 temperature sensors.
*/
void PIDHeater::setupTempSensor() {
  m_tempSensor.begin();

  uint8_t n_sensorsConnected = m_tempSensor.getDeviceCount();

  if (n_sensorsConnected < 1) {
    Serial.println("ERROR : No Temperature Sensor Connected");
  }
  
  m_tempSensor.setResolution(config::DS18B20_RESOLUTION_TEMPSENSOR);
  m_tempSensor.setWaitForConversion(false);  // async mode

  DallasTemperature::request_t req = m_tempSensor.requestTemperatures();

  if (!req.result) {
    Serial.println("ERROR : Temperature Sensor Request Timeout.");
  }

  m_tempSensorsDelay = 750 / (1 << (12 - config::DS18B20_RESOLUTION_TEMPSENSOR)); // ms

}


void PIDHeater::requestTemp() {
  if ( millis() - m_lastTempRequest > m_tempSensorsDelay) {
    //double temp = m_tempSensor.getTempCByIndex(0);
    double temp = m_tempSensor.getTempC(config::DS18B20_ADDRESS_THERMOBLOCK);
  
    if (temp == DEVICE_DISCONNECTED_C) {
      Serial.println("ERROR : DEVICE_DISCONNECTED_C");
    }
    else {
      m_currentTemperature = temp;
    }

    DallasTemperature::request_t req =  m_tempSensor.requestTemperatures(); // ask for next reading 

    if (!req.result) {
      Serial.println("ERROR : Temperature Sensor Request Timeout.");
    }

    m_lastTempRequest = millis();

    // set the temperature of sensor at index 0 to be the input of the PID
    m_pidInput = m_currentTemperature;
    
  }
}



/**
Turns on the Heating element of the Thermoblock by setting the duty cycle 
that switches the Solid State Relay to 100%.
*/
void PIDHeater::powerOn() {
  m_currentState = TBState::HEATING;
  m_SSR.switchOn();
}



/**
Turns off the Heating element of the Thermoblock by setting the duty cycle 
that switches the Solid State Relay to 0%.
*/
void PIDHeater::powerOff() {
  m_currentState = TBState::OFF;
  m_SSR.switchOff();
}



/**
Manually set the power percentage aka the duty cycle.
*/
void PIDHeater::power(double percentage) {
  m_SSR.setDutyCycle(percentage);
}


double PIDHeater::getTemperature() {
  return m_currentTemperature;
}


void PIDHeater::setTemperature(double setPoint) {
  if (setPoint >= config::TEMPERATURE_MAX) {
    Serial.println("ERROR : ERR_MAXTEMP");
    powerOff();
  }
  if (setPoint < 0) {
    Serial.println("ERROR : ERR_MINTEMP");
  }
  else {
    // heat up
    m_setTemperature = setPoint;
    m_pidSetpoint = setPoint;
  }
}



void PIDHeater::checkForSteadyState() {
  if (abs(m_pidSetpoint - m_pidInput) <= m_pidSetpointTolerance) {
      m_currentState = TBState::STEADYSTATE;
    }
  else {
    m_currentState = TBState::HEATING;
  }
}



void PIDHeater::update() {
  // read the current temperatures
  requestTemp();

  if (isHeating() || isSteadyState()) {
    updatePIDControl();
    checkForSteadyState();
  }
}


void PIDHeater::switchMode(TBMode mode) {
  switch (mode)
  {
  case TBMode::WARMUP:
    setPIDConstants(3.0, 1e-6, 0.0);
    break;

  case TBMode::IDLE:
    setPIDConstants(3.0, 1e-6, 0.0);
    break;

  case TBMode::PREBREW:
    setPIDConstants(5.0, 1e-5, 0.0);
    break;

  case TBMode::BREW:
    setPIDConstants(100.0, 1.0, 0.0);
    break;
  }
  m_mode = mode;
}


void PIDHeater::startPIDControl() {
  m_PID.setMode(true);              // turn on the PID controller
  m_currentState = TBState::HEATING;
}



void PIDHeater::stopPIDControl() {
  m_PID.setMode(false);             // turn off the PID controller
  powerOff();                       // just to make sure, turn off the SSR
  m_currentState = TBState::OFF;
}



void PIDHeater::setPIDConstants(double Kp, double Ki, double Kd) {
  m_Kp = Kp;
  m_Ki = Ki;
  m_Kd = Kd;

  m_PID.setTunings(m_Kp, m_Ki, m_Kd);
}


void PIDHeater::heatTo(double temperature, TBMode mode) {
  setTemperature(temperature);
  switchMode(mode);
  startPIDControl();
}


void PIDHeater::updatePIDControl() {

  if (m_pidInput == NAN) {        // in case auf faulty reading from the sensor shut down the heater
    powerOff();
    stopPIDControl();
    Serial.println("PID Control stopped due to NAN");
  }
  else {
    // determine the PID controlled variable.
    m_PID.compute();
    // adjust the duty cycle of the SSR.
    power(m_pidOutput / 255);
  }
  
}

double PIDHeater::getPIDinput() {return m_pidInput;}
double PIDHeater::getPIDsetpoint() {return m_pidSetpoint;}
double PIDHeater::getPIDouput() {return m_pidOutput;}


double PIDHeater::getPIDKp() {return m_PID.getKp();}
double PIDHeater::getPIDKi() {return m_PID.getKi();}
double PIDHeater::getPIDKd() {return m_PID.getKd();}

double PIDHeater::getDutyCycle() {return m_SSR.getDutyCycle();};

}