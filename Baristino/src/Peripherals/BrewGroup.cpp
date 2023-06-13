#include "BrewGroup.h"

// typename template this and use in isCurrentNominal
bool inRange(int16_t value, int16_t lowerBound, int16_t upperBound) {
    if (value >= lowerBound && value <= upperBound) {
        return true;
    }
    return false;
}


namespace BrewGroup {

Ensemble::Ensemble(uint8_t pinMotor1, uint8_t pinMotor2, uint8_t pinMotorEN, uint8_t pinEncoderA, uint8_t pinEncoderB, uint8_t pinAmpMeter) : 
m_motor(pinMotor1, pinMotor2, pinMotorEN),
m_encoder(pinEncoderA, pinEncoderB),
m_ampmeter(pinAmpMeter)
{
}


void Ensemble::initialize() {
    m_motor.initialize();
    m_encoder.initialize();
    m_ampmeter.initialize();
    m_motor.setSpeed(255);
}


void Ensemble::moveDown() {
    m_encoder.resetUpdateTimer();
    m_motor.rotate(true);
}


void Ensemble::moveUp() {
    m_encoder.resetUpdateTimer();
    m_motor.rotate(false);
}


bool Ensemble::checkIfPressed() {
    if (isStalled()) {
        m_motor.stop();
        m_currentState = BGState::PRESSED;
        m_transitionTo = BGState::IDLE;
        Serial.print("BrewGroup Pressed!");
        return true;
    }
    return false;
}


bool Ensemble::checkIfHomed() {
    if (isStalled()) {
        m_motor.stop();
        m_currentState = BGState::HOMED;
        m_transitionTo = BGState::IDLE;
        m_encoder.setOrigin();
        Serial.print("BrewGroup Homed!");
        return true;
    }
    return false;
}


bool Ensemble::checkIfOpened() {
    if (isMovingDown()) {
        if (m_encoder.getPosition() >= m_positionOpen-m_encoder.getTolerance()) { 
            m_motor.stop();
            m_lastState = m_currentState;
            m_currentState = BGState::OPENED;
            m_transitionTo = BGState::IDLE;
            Serial.print("BrewGroup Opened!");
            return true;
        }
    }
    return false;
}


bool Ensemble::isStalled() {
    if (m_motor.isRotating() && !hasPositionChanged()) { //&& !isCurrentNominal() && !hasPositionChanged()) {
        return true;
    }
    return false;
}

// the max allowed current is highly dependant on the state of the brewgroup.
// An initated movement from an idle state, HOMED or PRESSED requires more current
// compared to the free movement or forced stop due to end limit.
// PRESS -> MOVE > MOVE -> HOME
// HOME -> MOVE > MOVE -> PRESS
float Ensemble::getCurrentLimit() {
    
    if (m_currentState == BGState::PRESSED && m_transitionTo == BGState::HOMED) {
        return 2.2;
    }

    else if (m_transitionTo == BGState::PRESSED) {
        return 2.2;
    }
    else {
        return 2.0;
    }

}

int16_t Ensemble::getPosition() {
    return m_encoder.getPosition();
}


bool Ensemble::isHome() {return m_currentState == BGState::HOMED;}
bool Ensemble::isOpen() {return m_currentState == BGState::OPENED;}
bool Ensemble::isPress() {return m_currentState == BGState::PRESSED;}
bool Ensemble::isMoving() {return m_motor.isRotating();}
bool Ensemble::isMovingUp() {return m_motor.isRotatingCounterClockwise();}
bool Ensemble::isMovingDown() {return m_motor.isRotatingClockwise();}
bool Ensemble::isIdle() {return !isMoving();}

BGState Ensemble::getCurrentState() {return m_currentState;}
BGState Ensemble::getLastState() {return m_lastState;}



bool Ensemble::isCurrentNominal()
{
    double current = m_ampmeter.readCurrent();
    
    //if (abs(current) <= m_minCurrent || abs(current) >= m_maxCurrent) {
    //    return false;
    //}
    if (abs(current) <= getCurrentLimit()) {
        return true;
    } 

    return false;
}

// is it moving
bool Ensemble::hasPositionChanged() {
    return ((millis() - m_encoder.getLastUpdate()) <= m_stallTimeout);
}


double Ensemble::getCurrent() {
    return m_ampmeter.readCurrent();
}


void Ensemble::home() {
    if (!isHome()) {
        m_transitionTo = BGState::HOMED;
        moveUp();
        checkIfHomed();
    }
}


void Ensemble::open() {
    if (!isOpen()) {
        m_transitionTo = BGState::OPENED;
        moveDown();
        checkIfOpened();
    }
}

void Ensemble::press() {
    if (!isPress()) {
        m_transitionTo = BGState::PRESSED;
        moveDown();
        checkIfPressed();
    }
}


void Ensemble::update() {
    switch (m_transitionTo)
    {
    case BGState::IDLE:
        break;
    
    case BGState::HOMED:
        checkIfHomed();
        break;
    
    case BGState::OPENED:
        checkIfOpened();
        break;
    
    case BGState::PRESSED:
        checkIfPressed();
        break;

    default:
        break;
    }
}


void Ensemble::printState() {
    Serial.print("STATE : current = ");
    Serial.print((int)m_currentState);
    Serial.print(" TRANSITION ");
    Serial.print((int)m_lastState);
    Serial.print(" -> ");
    Serial.print((int)m_transitionTo);
    Serial.print(" | POSITION : ");
    Serial.print(m_encoder.getPosition());
    Serial.print(" | MOTOR CURRENT : ");
    Serial.print(getCurrent());
    Serial.print(" / ");
    Serial.println(getCurrentLimit());
}


}




// ROTARY ENCODER
RotaryEncoder::RotaryEncoder(uint8_t pinA, uint8_t pinB) : 
m_pinA(pinA), 
m_pinB(pinB) 
{
}


void RotaryEncoder::initialize()
{
    pinMode(m_pinA, INPUT);
    pinMode(m_pinB, INPUT);
    m_lastEncoderStateA = digitalRead(m_pinA);
    m_lastEncoderStateB = analogRead(m_pinB) > m_encoderThreshB ? 1 : 0;

    attachInterrupt(digitalPinToInterrupt(m_pinA), isrUpdate, CHANGE);
    instance = this;
}

void RotaryEncoder::setOrigin()
{
    m_currentPosition = 0;
    m_lastEncoderStateA = m_encoderStateA;
    m_lastEncoderStateB = m_encoderStateB;
}


void RotaryEncoder::isrUpdate() {
    RotaryEncoder::instance->updatePosition();
}


void RotaryEncoder::updatePosition() 
{
    m_encoderStateA = digitalRead(m_pinA);

    // the output B is behaving weird therefore we need to do this analog trickery
    m_encoderStateB = analogRead(m_pinB) > m_encoderThreshB ? 1 : 0;

    if (m_encoderStateA != m_lastEncoderStateA){     
    
        if (m_encoderStateB != m_encoderStateA) { 
            m_currentPosition--;
        } 
        else {
            m_currentPosition++;
        }
    }

    m_lastEncoderStateA = m_encoderStateA;

    m_lastUpdate = millis();
}


void RotaryEncoder::resetUpdateTimer() {
    m_lastUpdate = millis();
}


int16_t RotaryEncoder::getPosition()
{
    return m_currentPosition;
}



int16_t RotaryEncoder::getTolerance() {
    return m_stepTolerance;
}


int16_t RotaryEncoder::checkPositionValidity(int16_t targetPosition) {
    int16_t pos = 0;
    if (targetPosition < 0) {
        pos = 0;
    }
    else if (targetPosition > m_maxStep - m_stepTolerance) {
        pos = m_maxStep;
    }
    else {
        pos = targetPosition;
    }
    return pos;
}


unsigned long RotaryEncoder::getLastUpdate() {
    return m_lastUpdate;
}


RotaryEncoder *RotaryEncoder::instance;


// DC MOTOR
DCMotor::DCMotor(uint8_t pin1, uint8_t pin2, uint8_t pinEN) : 
m_pin1(pin1),
m_pin2(pin2),
m_pinEN(pinEN)
{
}



void DCMotor::initialize() 
{
    pinMode(m_pin1, OUTPUT);
    pinMode(m_pin2, OUTPUT);
    pinMode(m_pinEN, OUTPUT);
}


void DCMotor::setSpeed(uint8_t speed) 
{
    m_speed = speed;
}



void DCMotor::rotate(bool clockwise)
{
    analogWrite(m_pinEN, m_speed);

    if (clockwise) {
        digitalWrite(m_pin1, LOW);
        digitalWrite(m_pin2, HIGH);
        m_currentState = BGMotorState::CLOCKWISE;
    }
    else {
        digitalWrite(m_pin1, HIGH);
        digitalWrite(m_pin2, LOW);
        m_currentState = BGMotorState::COUNTERCLOCKWISE;
    }
}


void DCMotor::stop()
{
    analogWrite(m_pinEN, 0);
    digitalWrite(m_pin1, LOW);
    digitalWrite(m_pin2, LOW);
    m_currentState = BGMotorState::OFF;
}


bool DCMotor::isRotatingClockwise() {return m_currentState == BGMotorState::CLOCKWISE;}
bool DCMotor::isRotatingCounterClockwise() {return m_currentState == BGMotorState::COUNTERCLOCKWISE;}
bool DCMotor::isRotating() {return (isRotatingClockwise() || isRotatingCounterClockwise());}


// AMPMETER
AmpMeter::AmpMeter(uint8_t pinAnalogInput) : 
m_pinInput(pinAnalogInput)
{
}


void AmpMeter::initialize()
{
    pinMode(m_pinInput, INPUT);
}


double AmpMeter::readCurrent()
{
    double rawVoltage = (analogRead(m_pinInput) / 1024.0) * 5000;
    return (rawVoltage - m_V0) / m_VpA;
}
