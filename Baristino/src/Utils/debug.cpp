#include "debug.h"

void debug(DEBUGMODE mode, bool setup) {
    switch (mode)
    {
    case DEBUGMODE::NONE:
        break;
    
    case DEBUGMODE::ONOFFBUTTON_01:
        setup ? setup_ONOFFBUTTON_01() : loop_ONOFFBUTTON_01();
        break;

    default:
        break;
    }

}


void setup_ONOFFBUTTON_01() {
    pinMode(pindef::STDBY_BTN_VCC, OUTPUT);
    pinMode(pindef::STDBY_BTN_VCC, INPUT);

    digitalWrite(pindef::STDBY_BTN_VCC, HIGH);
} 

int stby_button;

void loop_ONOFFBUTTON_01() {
    stby_button = digitalRead(pindef::STDBY_BTN_IN);
    Serial.print("STBY BUTTON STATE : ");
    Serial.println(stby_button);
    delay(10);
}