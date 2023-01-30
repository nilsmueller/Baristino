#pragma once

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif


class RotaryEncoderInterrupt{

  public:
    RotaryEncoderInterrupt(uint8_t pinA, uint8_t pinB);
    void initialize();
    int16_t getPosition();
    void print();

  private:
    uint8_t m_pinA;
    uint8_t m_pinB;
    int16_t m_stepTolerance = 2;
    int16_t m_encoderStateA;
    int16_t m_encoderStateB;
    int16_t m_lastEncoderStateA;
    int16_t m_lastEncoderStateB;
    int16_t m_encoderThreshB = 860;
    volatile int16_t m_currentPosition;

    static void isrUpdate();
    void update();
    static RotaryEncoderInterrupt *instance;
};