#ifndef _COFFEEMACHINE_H
#define _COFFEEMACHINE_H

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif

struct BrewParam {
    double set_temperature = 93.0f;
    double set_volume = 150.0f;
    double set_dose = 12.0f;

    double current_temperature = 0.0;
    double current_volume = 0.0f;
    double current_dose = 0.0f;

    double start_temperature;
};


#include "configuration.h"
#include "statusTools.h"
#include "LCD/LCD.h"
#include "Peripherals/ThermoBlock.h"
#include "Peripherals/SSR.h"
#include "Peripherals/BrewGroup.h"
#include "Peripherals/Grinder.h"
#include "Peripherals/Pump.h"

#include <SPI.h>
#include <SD.h>
#include "Utils/SDCard.h"


enum class MachineState : uint8_t {
    IDLE,
    STDBY,
    WARMUP,
    COOLING,
    HEATING,
    GRINDING,
    BREWGROUP_INSERTION,
    TAMPERING,
    EXTRACTION,
    BREWGROUP_HOME,
    GRINDER_HOME,
    RETURN_TO_IDLE,
};


// TODO add Error Handling. The main Components are implemented. but the main design structure has yet to be thought of.

class CoffeeMachine {
    public:
        CoffeeMachine();
        void initialize();
        void warmup();
        void makeCoffee();
        void createStepResponse(double stepPower, bool waterFlow);
        void updateSensors();
        void printSensorValues();
        void updateLCD();

        void writeToFile();

    private:
        ThermoBlock::PIDHeater m_unitThermoBlock;
        WaterControl::Pump m_unitPump;
        Grinder::Hopper m_unitGrinder;
        BrewGroup::Ensemble m_unitBrewGroup;

        int16_t m_brewGroupPosition;
        double m_brewGroupCurrent;

        // Menu
        uint8_t m_currentMenuID = 0; // ID of the current Menu
        uint16_t m_newMenuID = 0;     // variable that stores the ID of a potential new Menu
        void changeMenu();

        MachineState m_currentState = MachineState::IDLE;

        unsigned long m_grinderFlapOpenedTimestamp;

        File m_file;
        bool m_SDCardEnabled = false;
        bool m_fileOpen = false;


        BrewParam m_brewParam;
        void resetBrewParam();
        void updateTemperature();
        void updateVolume();
        void updateDose();
        void updateBrewGroup();
        void setTemperature();
        void setVolume();
        void setDose();

};

#endif