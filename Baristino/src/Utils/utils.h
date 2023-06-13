#pragma once
#include <DallasTemperature.h>
#include <OneWire.h>
#include "../configuration.h"
#include <HX711.h>

// function to print a device address
void printDS18B20Address(DeviceAddress deviceAddress);
void printDS18B20AddressOCT(DeviceAddress deviceAddress);
void findDS18B20Sensors(const uint8_t busPin);

void calibrateScale(const uint8_t pinDOUT, const uint8_t pinSCK, double refMass); // in gramm

struct BrewParam {
    double set_temperature = 93.0f;
    double set_volume = 150.0;
    double set_dose = 12.0;
    double set_preinfusion = 2;// s
    double set_bloom = 5; // s

    double current_temperature = 0.0;
    double current_volume = 0.0f;
    double current_dose = 0.0f;

    double start_temperature;
    double current_dutyCycle = 0.0;
};


class EggTimer {
    public:
    EggTimer();
    EggTimer(unsigned long duration_ms);

    void setDurationMS(unsigned long duration_ms);
    unsigned long getDurationMS();
    unsigned long getLeftDurationMS();
    void startTimer();
    void stopTimer();
    void resetTimer();
    bool isActive();
    bool isFinished();
    void quickStart(unsigned long duration_ms);

    private:
    unsigned long m_startTimeMS;
    unsigned long m_durationMS;
    bool m_isFinished = false;
    bool m_isActive = false;
};
