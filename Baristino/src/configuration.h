#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif

#include <DallasTemperature.h>

namespace pindef {
    // Thermoblock
    const uint8_t SSR_THERMOBLOCK = 11;
    const uint8_t DS18B20_THERMOBLOCK = 23;

    // WaterPump
    const uint8_t  SSR_WATERPUMP = 13;

    // Grinder
    static const uint8_t SSR_GRINDER = 12;
    static const uint8_t HOPPER_LOADCELL_DOUT = 35;
    static const uint8_t HOPPER_LOADCELL_SCK = 34;
    static const int STEPPER_1 = 44;
    static const int STEPPER_2 = 46;
    static const int STEPPER_3 = 45;
    static const int STEPPER_4 = 47;

    // BrewGroup
    const int BREWGROUP_MOTOR_ENA = 42;
    const int BREWGROUP_MOTOR_IN_1 = 41;
    const int BREWGROUP_MOTOR_IN_2 = 40;
    const int BREWGROUP_MOTOR_AMPMETER = A15;
    const int BREWGROUP_ENCODER_INPUT_A = A13;
    const int BREWGROUP_ENCODER_INPUT_B = A14;

    // Scale
    const int BEVERAGE_LOADCELL_DOUT = 37;
    const int BEVERAGE_LOADCELL_SCK = 38;

    // Valve
    const int DRAIN_VALVE = 32;

    // TouchDisplay
    const int LCD_XP = 8;
    const int LCD_XM = A2;
    const int LCD_YP = A3;
    const int LCD_YM = 9;
}

namespace config {
// Generatal
static const unsigned long SLEEP_AFTER = 300000;

// Coffee Extraction
static const double VOLUME_MIN = 10.0;
static const double VOLUME_MAX = 200.0;
static const double TEMPERATURE_MIN = 50.0;
static const double TEMPERATURE_MAX = 120.0;
static const double QUANTITY_MIN = 5.0;
static const double QUANTITY_MAX = 20.0;
static const double EXTRACTION_LOADCELL_CALIBRATION_FACTOR = -1065.019512;

// Thermoblock
static const double TEMP_WARMUP = 100.0;
static const int PWM_RESOLUTION_SSR_THERMOBLOCK = 6;
static const double PWM_CARRIER_FREQUENCY = 50.0f;
static const int DS18B20_RESOLUTION_TEMPSENSOR = 10;
static const int DS18B20_NUM_TEMPSENSORS = 1;
static const DeviceAddress DS18B20_ADDRESS[DS18B20_NUM_TEMPSENSORS] = {0x28, 0xB3, 0x39, 0x75, 0xD0, 0x01, 0x3C, 0xDE};

// Grinder
static const double GRINDER_LOADCELL_CALIBRATION_FACTOR = -1083.78536585;
static const int STEPPER_STEPS_PER_REV = 2048;
static const int STEPPER_RPM = 5;
}

enum class ErrorCode : uint8_t {
    NONE = 0,
    ERR_TB_HEATINGFAIL,
    ERR_TB_TEMPABOVETHRESH,
    ERR_TB_TEMPBELOWTHRESH,
    ERR_TB_NOHEATING,
    ERR_TB_NOTEMPSENSOR,
    ERR_BG_MOTORSTALL,
    ERR_BG_ENCODERERROR,
};

enum class PumpState {
    OFF,
    ON,
    AUTO,
};

enum class GrinderState {
    OFF,
    ON,
    AUTO,
};

#endif