#include "BrewGroup.h"

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
    m_motor.rotate(true);
    m_currentState = BGState::MOVINGDOWN;
}

void Ensemble::moveUp() {
    m_motor.rotate(false);
    m_currentState = BGState::MOVINGUP;
}


bool Ensemble::checkIfPressed() {
    if (isMovingDown() && !isCurrentNominal()) {
        m_motor.stop();
        m_currentState = BGState::PRESSED;
        return true;

        // if position is somewhere around the desired position
        // return true
        // else false (motor stalled!)
    }
    return false;
}


bool Ensemble::checkIfHomed() {
    if (isMovingUp() && !isCurrentNominal()) {
        m_motor.stop();
        m_currentState = BGState::HOMED;
        m_encoder.setOrigin();
        Serial.print("BrewGroup Homed Successfully");
        return true;
    }
    return false;
}


bool Ensemble::checkIfOpened() {
    if (isMovingDown()) {
        if (inRange(m_encoder.getPosition(), m_positionOpen-m_encoder.getTolerance(), m_positionOpen+m_encoder.getTolerance())) {
            m_motor.stop();
            m_lastState = m_currentState;
            m_currentState = BGState::OPENED;
            return true;
        }
    }
    return false;
}



void Ensemble::home() {
    if (m_currentState == BGState::IDLE || m_currentState == BGState::OPENED || m_currentState ==  BGState::PRESSED) {
        
        while (isCurrentNominal()) {
            m_motor.rotate(false);
            Serial.print("HOME AMPS : ");
            Serial.print(m_ampmeter.readCurrent());
            Serial.print("  POS : ");
            Serial.println(m_encoder.getPosition());
            delay(10);
        }
        m_motor.stop();
        m_encoder.setOrigin();
        m_lastState = m_currentState;
        m_currentState = BGState::HOMED;
        Serial.print("BrewGroup Homed successfully!");
    }
}



// BLOCKING
void Ensemble::open() {
    moveToPosition(m_positionOpen);
    m_lastState = m_currentState;
    m_currentState = BGState::OPENED;
}

void Ensemble::press() {
    //moveToPosition(m_positionPress);
    while (isCurrentNominal()) {;
        m_motor.rotate(true);
        m_encoder.updatePosition();
        Serial.print("PRESS AMPS : ");
        Serial.print(m_ampmeter.readCurrent());
        Serial.print("  POS : ");
        Serial.println(m_encoder.getPosition());
        delay(10);
    }
    m_motor.stop();
    m_currentState = BGState::PRESSED;
    
}

void Ensemble::updatePosition() {
    m_encoder.updatePosition();
}


int16_t Ensemble::getPosition() {
    return m_encoder.getPosition();
}


bool Ensemble::isHome() {return m_currentState == BGState::HOMED;}
bool Ensemble::isOpen() {return m_currentState == BGState::OPENED;}
bool Ensemble::isPress() {return m_currentState == BGState::PRESSED;}
bool Ensemble::isMoving() {return (isMovingUp() || isMovingDown());}
bool Ensemble::isMovingUp() {return m_currentState == BGState::MOVINGUP;}
bool Ensemble::isMovingDown() {return m_currentState == BGState::MOVINGDOWN;}
bool Ensemble::isIdle() {return !isMoving();}

BGState Ensemble::State() {return m_currentState;}



bool Ensemble::isCurrentNominal()
{
    double current = m_ampmeter.readCurrent();

    if (isIdle()) {
        return (current >= m_minCurrentFromIdle && current < m_maxCurrentFromIdle) ? true : false;
    }
    else {
        return (current >= m_minCurrent && current < m_maxCurrent) ? true : false;
    }
    
    return false;
}


double Ensemble::getCurrent() {
    return m_ampmeter.readCurrent();
}

// PRIVATE
void Ensemble::moveToPosition(int16_t position) {
    int16_t targetPosition = m_encoder.checkPositionValidity(position);

    if (targetPosition > m_encoder.getPosition()) {
        m_motor.rotate(true);

        while (targetPosition > m_encoder.getPosition() + m_encoder.getTolerance()) {
            m_encoder.updatePosition();

            Serial.print("DOWN AMPS : ");
            Serial.print(m_ampmeter.readCurrent());
            Serial.print("  POS : ");
            Serial.println(m_encoder.getPosition());

            if (!isCurrentNominal()) {
                m_motor.stop();
                Serial.println("ERROR : Current Limit Exceeded. Motor has Stalled.");
                return;
            }
        }

        m_motor.stop();
    }

    else if (targetPosition < m_encoder.getPosition()) {
        m_motor.rotate(false);

        while (targetPosition < m_encoder.getPosition() - m_encoder.getTolerance()) {
            m_encoder.updatePosition();
            

            Serial.print("UP AMPS : ");
            Serial.print(m_ampmeter.readCurrent());
            Serial.print("  POS : ");
            Serial.println(m_encoder.getPosition());

            if (!isCurrentNominal()) {
                m_motor.stop();
                Serial.println("ERROR : Current Limit Exceeded. Motor has Stalled.");
                return;
            }
        }

        m_motor.stop();
    }

    else {
        m_motor.stop();
    }
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
}

void RotaryEncoder::setOrigin()
{
    m_currentPosition = 0;
    m_lastEncoderStateA = m_encoderStateA;
    m_lastEncoderStateB = m_encoderStateB;
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
    }
    else {
        digitalWrite(m_pin1, HIGH);
        digitalWrite(m_pin2, LOW);
    }

    m_isOn = true;
}


void DCMotor::stop()
{
    analogWrite(m_pinEN, 0);
    digitalWrite(m_pin1, LOW);
    digitalWrite(m_pin2, LOW);
    m_isOn = false;
}


bool DCMotor::isOn() {
    return m_isOn;
}


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
