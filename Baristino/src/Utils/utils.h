#pragma once
#include <DallasTemperature.h>
#include <OneWire.h>
#include "../configuration.h"

// function to print a device address
void printDS18B20Address(DeviceAddress deviceAddress);

void printDS18B20AddressOCT(DeviceAddress deviceAddress);

void findDS18B20Sensors(const uint8_t busPin);

struct BrewParam {
    double set_temperature = 93.0f;
    double set_volume = 150.0;
    double set_dose = 12.0;

    double current_temperature = 0.0;
    double current_volume = 0.0f;
    double current_dose = 0.0f;

    double start_temperature;
    double current_dutyCycle = 0.0;
};
