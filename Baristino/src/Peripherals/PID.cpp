#include "PID.h"


PID::PID(double *processVariable, double *controlVariable, double *setPoint, bool derivOnMeas) : 
  m_CV(controlVariable),
  m_PV(processVariable),
  m_SP(setPoint)
{
  dOnM = derivOnMeas;
  PID::setOutputLimits(0, 255);
  PID::setSampleTimeMS(100);
  PID::initialize();
}


/**
Initialize the PID controller so that the values of the previous computation step are not
initially arbitrary.
*/
void PID::initialize() {
  m_lastPV = *m_PV;
} 


/**
set the Mode of the PID Controller. If OnOff is '1', the PID controller is turned on.
Otherwise the PID controller is turned off and will not compute the control variable.
*/
void PID::setMode(int OnOff) {
  if (OnOff == 1) {
    m_mode = true;
  }
  else {
    m_mode = false;
  }
}


/**
Manually set the output limits of the PID controller. The default minimum value
is 0 and the default maximum value is 255.
*/
void PID::setOutputLimits(double minVal, double maxVal) {
  m_minOutput = minVal;
  m_maxOutput = maxVal;
}


/**
Set the proportional- (Kp), integral- (Ki) and derivative- (Kd) Gain.
*/
void PID::setTunings(double Kp, double Ki, double Kd) {
  m_KP = Kp;
  m_KI = Ki;
  m_KD = Kd;

  // TEST
  m_proportional = 0.0;
  m_integral = 0.0;
  m_derivative = 0.0;
}


/**
Set the sample time of the PID controller in milliseconds. The controller
will only compute the control variable every dt milliseconds.
*/
void PID::setSampleTimeMS(unsigned long dt) {
  m_dt = dt;
}


/**
function to clip the integral and output value. This will set the input value
either to m_minOutput or m_maxOutput if the value exceeds these limits respectively.
*/
double PID::clip(double value) {
  if (value < m_minOutput) {
    return m_minOutput;
  }
  else if (value > m_maxOutput) {
    return m_maxOutput;
  }
  else {
    return value;
  }
}


/**
the main function that computes the control variable for the given input. However, it will
only compute the output if the time duration between the last computation and now is larger
than the sample time.
*/
void PID::compute() {
  if(m_mode) {
    unsigned long now = millis();
    double m_dt_real = now - m_lastTimestamp;

    if (m_dt_real >= m_dt) {


      double PV = *m_PV;                    // input
      double d_PV = (PV - m_lastPV);        // input difference
      double error = *m_SP - PV;            // error 
      double d_error = error - m_lastError; // error difference

      // compute proportional term
      m_proportional = m_KP * error;

      // compute integral term
      m_integral += m_KI * error * m_dt_real;
      m_integral = PID::clip(m_integral);

      // compute derivative term
      if (dOnM) {
        m_derivative = -m_KD * d_PV / m_dt_real; // m_dt
      }
      else {
        m_derivative = m_KD * d_error / m_dt_real; // m_dt
      }

      // comput output
      double CV = m_proportional + m_integral + m_derivative;
      *m_CV = PID::clip(CV); 

      // keep track
      m_lastTimestamp = now;
      m_lastError = error;
      m_lastPV = PV;
    }
  }
}

double PID::getKp() {return m_KP;}
double PID::getKi() {return m_KI;}
double PID::getKd() {return m_KD;}