#ifndef _COFFEEMACHINE_H
#define _COFFEEMACHINE_H

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif


#include "configuration.h"
#include "Utils/utils.h"
#include "statusTools.h"
#include "LCD/LCD.h"
#include "Peripherals/ThermoBlock.h"
#include "Peripherals/SSR.h"
#include "Peripherals/BrewGroup.h"
#include "Peripherals/Grinder.h"
#include "Peripherals/Pump.h"

#include <SPI.h>
//#include <SD.h>
#include "Utils/SDCard.h"

// TODO 1. create return-to-idle routine
// TODO 2. create possibility to interrupt brew process
// TODO 3. add EggTimer class
// TODO 4. 


enum class MachineState : uint8_t {
    IDLE,
    STDBY,
    WARMUP,
    COOLING,
    HEATING,
    GRINDING,
    BREWGROUP_INSERTION,
    TAMPERING,
    PREINFUSION,
    PREINFUSION_PAUSE,
    EXTRACTION,
    BREWGROUP_HOME,
    GRINDER_HOME,
    RETURN_TO_IDLE,
    FLUSHING,
};


// TODO add Error Handling. The main Components are implemented. but the main design structure has yet to be thought of.

class CoffeeMachine {
    public:
        CoffeeMachine();
        void initialize();
        void warmup();
        void flush();
        void makeCoffee();
        void createStepResponse(double stepPower, bool waterFlow);
        void updateSensors();
        void printSensorValues();
        void updateLCD();

    private:
        ThermoBlock::PIDHeater m_unitThermoBlock;
        WaterControl::Pump m_unitPump;
        Grinder::Hopper m_unitGrinder;
        BrewGroup::Ensemble m_unitBrewGroup;
        mySDCard m_sdCard;

        int16_t m_brewGroupPosition;
        double m_brewGroupCurrent;

        // STBY Button
        void checkStandby();
        void goSleep();
        void wakeUp();
        long m_awakeSince;

        // Menu
        uint8_t m_currentMenuID = 0; // ID of the current Menu
        uint16_t m_newMenuID = 0;     // variable that stores the ID of a potential new Menu
        void changeMenu();

        MachineState m_currentState = MachineState::IDLE;

        // timer
        EggTimer m_timer = EggTimer();

        File m_file;
        bool m_SDCardEnabled = false;
        bool m_fileOpen = false;


        BrewParam m_brewParam;
        void resetBrewParam();
        void updateTemperature();
        void updateVolume();
        void updateDose();
        void updateBrewGroup();

        //
        //
        void returnToIdle();

};

#endif