#pragma once

#include <Stepper.h>
#include <HX711.h>
#include "../statusTools.h"
#include "../configuration.h"
//#include "SSR.h"



namespace Grinder
{

enum class Status : uint8_t  {
    SUCCESS,
    ERR_THERMALOVERLOAD,
    ERR_STEPPERSTALLED,
    ERR_SCALEVALUE,
};

enum class GRState : uint8_t {
    IDLE,
    OPEN,
    CLOSE,
    GRINDING,
    FINISHED,
};


using doubleResult = StatusResult<Status, double>;



class Hopper {
    public:
        Hopper(uint16_t stepsPerRev, uint8_t pinStepper1, uint8_t pinStepper2, uint8_t pinStepper3, uint8_t pinStepper4, uint8_t pinLoadCellDOUT, uint8_t pinLoadCellSCK, uint8_t pinSSR);

        // GENERAL
        void initialize();
        bool isIdle();
        bool isClose();
        bool isOpen();
        bool isGrinding();
        bool isFinished();

        double getCurrentAmount();

        // MANUAL CONTROL
        void homeFlap();
        void openFlap();
        void closeFlap();
        void setDose(double amount);
        void tareScale();
        void updateScale();
        void switchMotorOn();
        void switchMotorOff();

        GRState getState();

        // AUTOMATIC CONTROL
        void grind(double amount);


    private:
        // peripheral devices
        Stepper m_stepperMotor;
        HX711 m_scale;
        //SolidStateRelay m_grinderMotor;

        uint16_t m_stepsPerRev = 2048;
        double m_stepsPerDeg;
        double m_targetAmount = 1.0;
        double m_scaleCalibrationFactor = 0.0;
        int m_openingAngle = 110.0;
        int m_currentFlapPosition = 0;
        double m_currentAmount = 0.0;

        unsigned long m_updateRequestMillis = 100; // update every 100 milliseconds
        unsigned long m_lastScaleRequest = millis(); 

        // Pins
        uint8_t m_pinStepper1;
        uint8_t m_pinStepper2;
        uint8_t m_pinStepper3;
        uint8_t m_pinStepper4;
        uint8_t m_pinMotor1;
        uint8_t m_pinLoadCellSCK;
        uint8_t m_pinLoadCellDOUT;

        // FSM
        GRState m_currentState = GRState::IDLE;
        void checkForFinshed();

        void setScaleCalibrationFactor(double calibrationFactor);

};

} // namespace Grinder