#include "TimerOne.h"
#include "Arduino.h"
#include "HardwareSerial.h"
#include "SSR.h"

// TODO This class only allows a single PWM single to be generated. Right now the workaround is to handle
// TODO SSR like simple digtialWrite when no carrier frequency is given (and therefor no PWM capabilites).

SolidStateRelay::SolidStateRelay(uint8_t pin) {
  m_pin = pin;
  pinMode(pin, OUTPUT);
  m_dutyCycle = 1.0;

  // REMOVE THIS LATER
  m_periodCarrier = 0;
}

// constructor
SolidStateRelay::SolidStateRelay(uint8_t pin, uint16_t pwm_resolution, double carrier_freq) {
  m_pin = pin;
  m_resolutionPWM = pwm_resolution;                 // bit, the resolution of the pwm signal
  m_periodCarrier = (1 / carrier_freq) * 1000000;   // us, period of the carrier signal
  m_periodBit = m_periodCarrier / 2;                // us, period of a half wacve which is 1 Bit in case of the SSR
  m_numValues = pow(2, m_resolutionPWM) + 1;        // int. number of valid duty cycles
  m_periodPWM = m_periodBit * m_numValues;          // us, duration of the pwm signal

  // init array size
  m_validDutyCycles = (double*)calloc((m_numValues + 1), sizeof(double));

  // determine the valid duty cycles for the specified resolution
  double res = 1.0 / ((double)m_numValues - 1);
  
  for (unsigned int i = 0; i < m_numValues + 1; i++) {
        m_validDutyCycles[i] = i*res;
  }

}

void SolidStateRelay::initialize() {
    // set TIMER1 frequency
  Serial.print("PIN : ");
  Serial.print(m_pin);
  Serial.print(" PERIOD : ");
  Serial.print(m_periodPWM);
  
  Timer1.initialize(m_periodPWM);   // set the timer1 frequency according to the resolution and carrier frequency
  Timer1.pwm(m_pin, 0);           // set init duty cycle to 0
  
}

// destructor
SolidStateRelay::~SolidStateRelay() {
  free(m_validDutyCycles); 
  m_validDutyCycles = NULL;
}


/**
Turns on the Heating element of the Thermoblock by setting the duty cycle 
that switches the Solid State Relay to 100%.
*/
void SolidStateRelay::switchOn() {
  digitalWrite(m_pin, HIGH); 
  m_isOn = true;
  //Timer1.setPwmDuty(PIN_SSR, 1.0); // PID has to be running for this
}


/**
Turns off the Heating element of the Thermoblock by setting the duty cycle 
that switches the Solid State Relay to 0%.
*/
void SolidStateRelay::switchOff() {
  digitalWrite(m_pin, LOW);
  m_isOn = false;
  //Timer1.setPwmDuty(PIN_SSR, 0.0); // PID has to be running for this
}


bool SolidStateRelay::isOn() {
  return m_isOn;
}

/**
Returns the valud duty cycle value, which is determined by the resolution of the pwm controller.
The value must lie between 0 and 1.

@param value the desired duty cycle
@return return_val a valid duty cycle which is closest to the desired input.
*/
double SolidStateRelay::getValidDutyCycle(double value) {
  // find the closest valid duty cycle for the given input value, which must lie between 0 and 1;
  if (value > 1.0) {
    Serial.println("value > 1. The value must lie between 0 and 1. The value is set to 1.");
    value = 1.0;
  }
  else if (value < 0) {
    Serial.println("value < 0. The value must lie between 0 and 1. The value is set to 0.");
    value = 0.0;
  }

  double closest_value = abs(m_validDutyCycles[0] - value);
  double return_val = 0.0f;

  for (unsigned int i=0; i < m_numValues; i++) {

    double absdiff = abs(m_validDutyCycles[i] - value);


    if (absdiff < closest_value) {
        closest_value = absdiff;
        return_val = m_validDutyCycles[i];
    }

  }
  return return_val;
}


/**
Set the duty cycle of the PIN_SSR output pin.

@param value the desired duty cycle.
*/
void SolidStateRelay::setDutyCycle(double value) {
  
  m_dutyCycle = getValidDutyCycle(value);
  m_widthPulse = (int)m_dutyCycle * m_periodPWM;
  Timer1.setPwmDuty(m_pin, m_dutyCycle * 1023);

  //if (m_dutyCycle != m_lastDutyCycle) {
  //  m_widthPulse = (int)m_dutyCycle * m_periodPWM;
  //  Timer1.setPwmDuty(m_pin, m_dutyCycle * 1023);
  //  m_lastDutyCycle = m_dutyCycle;
  //}
}

/**
Get the current set valid duty cycle.

@return m_dutyCycle the current duty cycle.
*/
double SolidStateRelay::getDutyCycle() {
  return m_dutyCycle;
}


/**
Get the current set Pulse width in microseconds.

@return m_widthPulse the current pulse width.
*/
unsigned long SolidStateRelay::getPulseWidth() {
  return m_widthPulse;
}


/**
Get the current Period of the PWM Signal in microseconds.
The period length is determined by the carrier frequency
and the specified resolution.

@return m_periodPWM the pwm signal period
*/
unsigned long SolidStateRelay::getPWMPeriod() {
  return m_periodPWM;
}


/**
Get the current period a of a single 'bit' or half wave
of the carrier signal in microseconds.

@return m_periodBit The length of a single bit.
*/
unsigned long SolidStateRelay::getBitPeriod() {
  return m_periodBit;
}
