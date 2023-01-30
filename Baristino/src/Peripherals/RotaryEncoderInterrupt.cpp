#include "RotaryEncoderInterrupt.h"


// ROTARY ENCODER
RotaryEncoderInterrupt::RotaryEncoderInterrupt(uint8_t pinA, uint8_t pinB) : 
m_pinA(pinA), 
m_pinB(pinB) 
{
}


void RotaryEncoderInterrupt::initialize()
{
    pinMode(m_pinA, INPUT);
    pinMode(m_pinB, INPUT);
    m_lastEncoderStateA = digitalRead(m_pinA);
    m_lastEncoderStateB = analogRead(m_pinB) > m_encoderThreshB ? 1 : 0;

    attachInterrupt(digitalPinToInterrupt(m_pinA), isrUpdate, CHANGE);
    instance = this;
}


void RotaryEncoderInterrupt::isrUpdate() {
    RotaryEncoderInterrupt::instance->update();
}

void RotaryEncoderInterrupt::update() {
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

    Serial.print("Position : ");
    Serial.println(m_currentPosition);
}


int16_t RotaryEncoderInterrupt::getPosition() {
    return m_currentPosition;
}

void RotaryEncoderInterrupt::print() {
    Serial.print("A = ");
    Serial.print(m_encoderStateA);
    Serial.print(", B = ");
    Serial.print(m_encoderStateB);
    Serial.print(", Pos = ");
    Serial.print(m_currentPosition);
    Serial.println();
}

RotaryEncoderInterrupt *RotaryEncoderInterrupt::instance;