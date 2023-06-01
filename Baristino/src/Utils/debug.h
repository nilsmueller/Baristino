#pragma once

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif

#include "../configuration.h"

enum class DEBUGMODE : uint8_t {
    NONE,
    BREWUNIT_01,
    ONOFFBUTTON_01,
};


void debug(DEBUGMODE mode, bool setup);


void setup_ONOFFBUTTON_01();
void loop_ONOFFBUTTON_01();