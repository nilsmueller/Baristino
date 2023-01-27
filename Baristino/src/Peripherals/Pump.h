#pragma once
#include <HX711.h>
//#include "SSR.h"
#include "../configuration.h"
#include "../statusTools.h"

namespace WaterControl {

enum class Status : uint8_t  {
    SUCCESS,
    ERR_SCALEVALUE,
};

enum class WCState : uint8_t {
    OFF,
    ON,
};


class Pump {
    public:
        Pump(uint8_t pinSSR, uint8_t pinLoadCellSCK, uint8_t pinLoadCellDOUT);
        
        void initialize();
        bool isFinished();
        bool isOff();
        bool isOn();
        WCState getState();

         // automatic control
        void update();
        void pumpAmount(double amount);

        // manual control
        void tareScale();
        void updateScale();
        void setAmount(double amount);
        double getCurrentAmount();
        double getSetAmount();
        void switchOn();
        void switchOff();


    private:
        //SolidStateRelay m_SSR;
        HX711 m_scale;
        uint8_t pinSSR;
        uint8_t pinLoadCellSCK;
        uint8_t pinLoadCellDOUT;

        double m_setAmount = 1.0;
        double m_currentAmount = 0.0;
        bool m_isFinished = false;
        double m_scaleCalibrationFactor = 0.0;

        unsigned long m_updateRequestMillis = 100; // update every 100 milliseconds
        unsigned long m_lastScaleRequest = millis();      

        uint8_t m_pinSSR;
        uint8_t m_pinLoadCellSCK;
        uint8_t m_pinLoadCellDOUT;

        // FSM
        WCState m_currentState = WCState::OFF;

        void setScaleCalibrationFactor(double calibrationFactor);
};

} // namespace WaterControl