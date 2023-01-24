#ifndef _SSR_H
#define _SSR_H

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif


// pseudo PWM controlled Solid State Relay
class SolidStateRelay {
  public:
    SolidStateRelay(uint8_t pin);
    SolidStateRelay(uint8_t pin, uint16_t resolution, double carrierFrequency);
    ~SolidStateRelay();
    
    void initialize();
    void switchOn();
    void switchOff();
    bool isOn();
    void setDutyCycle(double value);
    double getDutyCycle();
    unsigned long getPulseWidth();
    unsigned long getPWMPeriod();
    unsigned long getBitPeriod();


  private:
    uint8_t m_pin;
    bool m_isOn = false;
    double m_periodCarrier = 0;                  // us, period of carrier signal
    double m_periodBit;                      // us, duration of single pulse (or quasi Bit)
    uint16_t m_resolutionPWM;           // bit, resolution of the SSR pwm, e.g. 2 = (0, 0.25, 0.5, 0.75) etc.
    uint16_t m_numValues;               // num of valid duty cycles
    unsigned long m_periodPWM;              // us , pwm signal period
    unsigned long m_widthPulse;             // us, pwm pulse width
    double *m_validDutyCycles;               // , array of all the valid duty cycles
    double m_dutyCycle;                      // [0 - 1], the current duty cycle
    double m_lastDutyCycle = 0.0;
    double m_currentDutyCycle;

    double getValidDutyCycle(double value);
};

#endif