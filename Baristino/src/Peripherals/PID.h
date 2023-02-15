#ifndef _PID_H
#define _PID_H

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif


class PID {
  public:
    // constructor
    PID(double *processVariable, double *controlVariable, double *setPoint, bool derivOnMeas);

    // public variables
    bool dOnM = true;         // if true, the derivative of Measurement is computed. Otherwise derivative of error.

    // public methods
    void initialize();
    void setOutputLimits(double minVal, double maxVal);
    void setMode(int OnOff);
    void setTunings(double Kp, double Ki, double Kd);
    void setSampleTimeMS(unsigned long dt);
    void compute();

    double getKp();
    double getKi();
    double getKd();

  private:
    bool m_mode = false;

    // PID constants
    double m_KP = 0.0;
    double m_KI = 0.0;
    double m_KD = 0.0;

    // variables
    double *m_CV; // control variable
    double *m_PV; // process variable
    double *m_SP; // setpoint

    // output limits
    double m_minOutput = 0;
    double m_maxOutput = 255;

    // sample time in milliseconds
    unsigned long m_dt; 

    double m_lastPV;
    double m_lastError;
    double m_lastTimestamp;
    double m_proportional = 0.0;
    double m_integral = 0.0;
    double m_derivative = 0.0;

    double clip(double value);
};


#endif