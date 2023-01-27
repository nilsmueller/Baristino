#pragma once

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif


#include "../configuration.h"
#include "../statusTools.h"

// TODO remove the dependency on "configuration.h"

// DEVICE CLASSES
class RotaryEncoder{
    public:
        RotaryEncoder(uint8_t pinA, uint8_t pinB);

        void initialize();
        void updatePosition();
        int16_t getPosition();
        int16_t getTolerance();
        int16_t checkPositionValidity(int16_t targetPosition);
        void setOrigin();

    private:
        uint8_t m_pinA;
        uint8_t m_pinB;
        int16_t m_currentPosition = 1;
        int16_t m_stepTolerance = 2;
        int16_t m_encoderStateA;
        int16_t m_encoderStateB;
        int16_t m_lastEncoderStateA;
        int16_t m_lastEncoderStateB;
        int16_t m_encoderThreshB = 860;
        int16_t m_maxStep = 324;
};


class DCMotor{
    public:
        DCMotor(uint8_t pin1, uint8_t pin2, uint8_t pinEN);

        void initialize();
        void setSpeed(uint8_t speed);
        void rotate(bool clockwise = true);
        void stop();
        bool isOn();

    private:
        uint8_t m_pin1;
        uint8_t m_pin2;
        uint8_t m_pinEN;
        uint8_t m_speed;

        bool m_isOn;
};


class AmpMeter{
    public:
        AmpMeter(uint8_t pinAnalogInput);
        void initialize();
        double readCurrent();

    private:
        uint8_t m_pinInput;
        double m_VpA = 66; // Volts per Amps
        double m_V0 = 2500.0; // mV, voltage that equals 0 A reading
};


namespace BrewGroup
{

enum class Status : uint8_t {
    SUCCESS,
    ERR_MOTORSTALL,
    ERR_ENCODERVALUE,
};

enum class BGState : uint8_t {
    IDLE,
    MOVINGUP,
    MOVINGDOWN,
    HOMED,
    OPENED,
    PRESSED,
};

class Ensemble {
    public:
        Ensemble(uint8_t pinMotor1, uint8_t pinMotor2, uint8_t pinMotorEN, uint8_t pinEncoderA, uint8_t pinEncoderB, uint8_t pinAmpMeter);

        void initialize();
        void home();
        void open();
        void press();
        void updatePosition();
        int16_t getPosition();

        bool isHome();
        bool isOpen();
        bool isPress();
        bool isIdle();
        bool isMoving();
        bool isMovingUp();
        bool isMovingDown();
        BGState State();

        void moveDown();
        void moveUp();
        bool isCurrentNominal();
        double getCurrent();

        bool checkIfHomed();
        bool checkIfOpened();
        bool checkIfPressed();

        
    private:
        DCMotor m_motor;
        RotaryEncoder m_encoder;
        AmpMeter m_ampmeter;

        int16_t m_positionOpen = 60;
        int16_t m_positionPress = 200;


        double m_minCurrent = -2.6; // A // downward movement (positive step directio)
        double m_maxCurrent = 2.3; // A // upward movement (negative step directio)
        double m_minCurrentFromIdle = -3.5;
        double m_maxCurrentFromIdle = 3.5;

        // FSM
        BGState m_currentState = BGState::IDLE;
        BGState m_lastState = BGState::IDLE;

        void moveToPosition(int16_t position);
        
};

}; // namespace BrewGroup

