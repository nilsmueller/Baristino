#include "Pump.h"

namespace WaterControl {

Pump::Pump(uint8_t pinSSR, uint8_t pinLoadCellSCK, uint8_t pinLoadCellDOUT) :
//m_SSR(pinSSR),
m_pinSSR(pinSSR),
m_pinLoadCellSCK(pinLoadCellSCK),
m_pinLoadCellDOUT(pinLoadCellDOUT)
{
}

void Pump::initialize() {
    //m_SSR.initialize();
    pinMode(m_pinSSR, OUTPUT);
    digitalWrite(m_pinSSR, LOW);
    m_currentState = WCState::OFF;
    
    m_scale.begin(m_pinLoadCellDOUT, m_pinLoadCellSCK);

    m_scaleCalibrationFactor = config::EXTRACTION_LOADCELL_CALIBRATION_FACTOR;
    m_scale.set_scale(config::EXTRACTION_LOADCELL_CALIBRATION_FACTOR);
    tareScale();
}


bool Pump::isOff() {return m_currentState == WCState::OFF;}
bool Pump::isOn() {return m_currentState == WCState::ON;}

WCState Pump::getState() {return m_currentState;}


void Pump::update() {
    switch (m_currentState)
    {
    case WCState::OFF:
        break;
    
    case WCState::ON:
        updateScale();
        if (isFinished()) {
            switchOff();
        }
        break;
    
    default:
        break;
    }
}


void Pump::tareScale() {
    m_scale.tare();
    m_currentAmount = 0.0;
}


void Pump::updateScale() {
    //m_currentAmount = m_scale.get_units(1);

    if (millis() - m_lastScaleRequest >  m_updateRequestMillis) {
        m_currentAmount = m_scale.get_units(1);
        m_lastScaleRequest = millis();
    }
    else {
        Serial.println("Pump Scale Reading outdated!");
        // return ERROR or so
    }
}


void Pump::setAmount(double amount) {
    m_setAmount = amount;
}


double Pump::getCurrentAmount() {
    return m_currentAmount;
}


void Pump::switchOff() {
    //m_SSR.switchOff();
    digitalWrite(m_pinSSR, LOW);
    m_currentState = WCState::OFF;
}


void Pump::switchOn() {
    //m_SSR.switchOn();
    digitalWrite(m_pinSSR, HIGH);
    m_currentState = WCState::ON;
}


bool Pump::isFinished() {
    return (m_currentAmount < m_setAmount) ? false : true;
}



void Pump::pumpAmount(double amount) {
    setAmount(amount);
    delay(200);
    tareScale();
    delay(2000);

    switchOn();
    
    while (!isFinished()) {
        updateScale();
        Serial.print("Current Amount Pumped : ");
        Serial.print(m_currentAmount);
        Serial.println(" [ml]");
        switchOn();
    }

    switchOff();
}


void Pump::setScaleCalibrationFactor(double calibrationFactor) {
    m_scaleCalibrationFactor = calibrationFactor;
    m_scale.set_scale(m_scaleCalibrationFactor);
}

}