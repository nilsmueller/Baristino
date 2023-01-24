#ifndef _COFFEEMACHINE_H
#define _COFFEEMACHINE_H

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif


#include "configuration.h"
#include "statusTools.h"
#include "LCD/LCD.h"
#include "Peripherals/ThermoBlock.h"
#include "Peripherals/SSR.h"
#include "Peripherals/BrewGroup.h"
#include "Peripherals/Grinder.h"
#include "Peripherals/Pump.h"

enum class MachineState : uint8_t {
    IDLE,
    STDBY,
    WARMUP,
    HEATING,
    GRINDING,
    BREWGROUP_INSERTION,
    TAMPERING,
    EXTRACTION,
    RETURN_TO_IDLE,
};

// TODO add Error Handling. The main Components are implemented. but the main design structure has yet to be thought of.

class CoffeeMachine {
    public:
        CoffeeMachine();
        void initialize();
        void warmup();
        void makeCoffee();
        void makeCoffeeNonBlocking();

        void fillSystem();
        void flush();

    private:
        ThermoBlock::PIDHeater m_unitThermoBlock;
        WaterControl::Pump m_unitPump;
        Grinder::Hopper m_unitGrinder;
        BrewGroup::Ensemble m_unitBrewGroup;
        
        double m_setVolume = 100.0;
        double m_setTemperature = 99.0;
        double m_setQuantity = 10.0;
        double m_currentVolume = 0.0;
        double m_currentTemperature;
        double m_currentQuantity;

        // Menu
        uint8_t m_currentMenuID = 0; // ID of the current Menu
        uint8_t m_newMenuID = 0;     // variable that stores the ID of a potential new Menu
        long m_lastTouch = millis();


        void updateSensors();
        
        bool m_brewState = false;
        MachineState m_currentState = MachineState::IDLE;

        unsigned long m_grinderFlapOpenedTimestamp;
};

#endif