#include "Grinder.h"

namespace Grinder {

// constructor
Hopper::Hopper(uint16_t stepsPerRev, uint8_t pinStepper1, uint8_t pinStepper2, uint8_t pinStepper3, uint8_t pinStepper4, uint8_t pinLoadCellDOUT, uint8_t pinLoadCellSCK, uint8_t pinSSR) : 
 m_stepperMotor(stepsPerRev, pinStepper1, pinStepper2, pinStepper3, pinStepper4),
 m_pinMotor1(pinSSR)
 //m_grinderMotor(pinSSR)
{
    m_stepsPerRev = stepsPerRev;
    m_stepsPerDeg = (double)stepsPerRev / 360.0;
    m_pinStepper1 = pinStepper1;
    m_pinStepper2 = pinStepper2;
    m_pinStepper3 = pinStepper3;
    m_pinStepper4 = pinStepper4;
    m_pinMotor1 = pinSSR;
    m_pinLoadCellSCK = pinLoadCellSCK;
    m_pinLoadCellDOUT = pinLoadCellDOUT;
}

void Hopper::initialize() {
    //m_grinderMotor.initialize();
    pinMode(m_pinMotor1, OUTPUT);
    digitalWrite(m_pinMotor1, LOW);

    m_scale.begin(m_pinLoadCellDOUT, m_pinLoadCellSCK),

    m_stepperMotor.setSpeed(config::STEPPER_RPM);


    m_scaleCalibrationFactor = config::GRINDER_LOADCELL_CALIBRATION_FACTOR;
    m_scale.set_scale(config::GRINDER_LOADCELL_CALIBRATION_FACTOR);
    m_scale.tare();
    homeFlap();
}

// GENERAL
bool Hopper::isIdle() {return m_currentState == GRState::IDLE;}
bool Hopper::isOpen() {return m_currentState == GRState::OPEN;}
bool Hopper::isClose() {return m_currentState == GRState::CLOSE;}
bool Hopper::isGrinding() {return m_currentState == GRState::GRINDING;}
bool Hopper::isFinished() {return m_currentState == GRState::FINISHED;}


GRState Hopper::getState() {
    return m_currentState;
}


void Hopper::checkForFinshed() {
    if (isGrinding()) {
        if (m_currentAmount >= m_targetAmount) {
            switchMotorOff();
            m_currentState = GRState::FINISHED;
        }
    }
}


double Hopper::getCurrentAmount() {
    return m_currentAmount;
}


// MANUAL CONTROL
void Hopper::homeFlap() {
    m_stepperMotor.step(-m_stepsPerDeg * (m_openingAngle + 10));
    m_currentState = GRState::CLOSE;
}


void Hopper::openFlap() {
    if (isFinished()) {
        m_stepperMotor.step(m_stepsPerDeg * m_openingAngle);
        m_currentState = GRState::OPEN;
    }
}


void Hopper::closeFlap() {
    if (isOpen()){
        m_stepperMotor.step(m_stepsPerDeg * -m_openingAngle);
        m_currentState = GRState::CLOSE;
    }
}


void Hopper::setDose(double amount) {
    m_targetAmount = amount;
}


void Hopper::tareScale() {
    m_scale.tare();
    m_currentAmount = 0.0;
}


void Hopper::updateScale() {
    if (millis() - m_lastScaleRequest >  m_updateRequestMillis) {
        m_currentAmount = m_scale.get_units(1);
    }
    else {
        Serial.println("Grinder Scale Reading outdated!");
    }
}



void Hopper::switchMotorOn() {
    //m_grinderMotor.switchOn();
    if (isClose()){ 
        digitalWrite(m_pinMotor1, HIGH);
        m_currentState = GRState::GRINDING;
    }
}


void Hopper::switchMotorOff() {
    //m_grinderMotor.switchOff();
    if (isGrinding()) {
        digitalWrite(m_pinMotor1, LOW);
        m_currentState = GRState::FINISHED;
    }
}

// AUTOMATIC CONTROL
void Hopper::grind(double amount) {
    Serial.println("Setting Dose");
    setDose(amount);
    Serial.println("Closing Flap");
    homeFlap();
    delay(100);
    tareScale();
    Serial.println("Taring");
    delay(100);
    Serial.println("grrrinding");

    switchMotorOn();

    while (!isFinished()) {
        updateScale();
        checkForFinshed();
        Serial.println(m_currentAmount);
    } 

    switchMotorOff();

    delay(1000);

    Serial.println("Opening Flap");
    openFlap();
    delay(1000);

    Serial.println("Closing Flap");
    closeFlap();
}


// PRIVATE
void Hopper::setScaleCalibrationFactor(double calibrationFactor) {
    m_scaleCalibrationFactor = calibrationFactor;
    m_scale.set_scale(m_scaleCalibrationFactor);
}


}