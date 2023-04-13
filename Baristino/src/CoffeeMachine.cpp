#include "CoffeeMachine.h"


// Constructor with member init
CoffeeMachine::CoffeeMachine() :
m_unitThermoBlock(pindef::SSR_THERMOBLOCK, pindef::DS18B20_THERMOBLOCK),
m_unitPump(pindef::SSR_WATERPUMP, pindef::BEVERAGE_LOADCELL_SCK, pindef::BEVERAGE_LOADCELL_DOUT),
m_unitGrinder(config::STEPPER_STEPS_PER_REV, pindef::STEPPER_1, pindef::STEPPER_2, pindef::STEPPER_3, pindef::STEPPER_4, pindef::HOPPER_LOADCELL_DOUT, pindef::HOPPER_LOADCELL_SCK, pindef::SSR_GRINDER),
m_unitBrewGroup(pindef::BREWGROUP_MOTOR_IN_1, pindef::BREWGROUP_MOTOR_IN_2, pindef::BREWGROUP_MOTOR_ENA, pindef::BREWGROUP_ENCODER_INPUT_A, pindef::BREWGROUP_ENCODER_INPUT_B, pindef::BREWGROUP_MOTOR_AMPMETER),
m_sdCard(pindef::CHIP_SELECT)
{
}


// initialization of peripheral devices
void CoffeeMachine::initialize() {
    LCD::initialize();
    m_unitThermoBlock.initialize();
    m_unitPump.initialize();
    m_unitGrinder.initialize();
    m_unitBrewGroup.initialize();
    m_sdCard.initialize();
}



void CoffeeMachine::warmup() {

    LCD::drawWarmUpScreen();
    m_unitThermoBlock.heatTo(config::TEMP_IDLE, ThermoBlock::TBMode::WARMUP);
    m_sdCard.setupWarmupFile();

    double startTemp = m_brewParam.current_temperature = m_unitThermoBlock.getTemperature();
    double progress;    
    double val = 0.0f;
    while (!m_unitThermoBlock.isSteadyState()) {
        m_unitThermoBlock.update();
        m_brewParam.current_temperature = m_unitThermoBlock.getTemperature();
        //progress = (config::TEMP_IDLE - abs(config::TEMP_IDLE - m_brewParam.current_temperature)) / config::TEMP_IDLE;
        val = abs((config::TEMP_IDLE - m_brewParam.current_temperature) / (config::TEMP_IDLE - startTemp));
        progress = 1 - max(0, min(1, val));
        LCD::updateWarmUpScreen(progress);

        // print status
        Serial.print("WARMUP - Temp = ");
        Serial.print(m_brewParam.current_temperature);
        Serial.print(" / ");
        Serial.print(config::TEMP_IDLE);
        Serial.print(" [°C] ");
        Serial.print(" | Position : ");
        Serial.print(m_unitBrewGroup.getPosition());
        Serial.print(" | Current : ");
        Serial.print(m_unitBrewGroup.getCurrent());
        Serial.println();

        m_sdCard.writeFile(&m_brewParam);

        delay(10);
    }

    if (m_SDCardEnabled) {
        m_file.close();
    }
    
    // 1. Move Down and flush
    flush();

    // 2. home brew group
    Serial.println("BREWGROUP_HOMING ");
    m_unitBrewGroup.moveUp();
    while (m_unitBrewGroup.isMovingUp()) {
        m_unitBrewGroup.checkIfHomed();
    }

    delay(1000);

    // 3. open brew group
    Serial.println("BREWGROUP_OPENING");
    m_unitBrewGroup.moveDown();
    while (m_unitBrewGroup.isMovingDown()) {
        m_unitBrewGroup.checkIfOpened();
    }
    
    // 4. switch to main menu
    LCD::drawMainMenu();
}


void CoffeeMachine::updateTemperature() {
    m_unitThermoBlock.update();
    m_brewParam.current_temperature = m_unitThermoBlock.getTemperature();
    m_brewParam.current_dutyCycle = m_unitThermoBlock.getPIDouput();
}


void CoffeeMachine::updateVolume() {
    if (m_currentState >= MachineState::TAMPERING) { //m_currentState == MachineState::TAMPERING || m_currentState == MachineState::EXTRACTION || m_currentState == MachineState::RETURN_TO_IDLE) {
        m_unitPump.updateScale();
        m_brewParam.current_volume = max(m_brewParam.current_volume, m_unitPump.getCurrentAmount());
    }
    else {
        m_brewParam.current_volume = 0.0f;
    }
}

void CoffeeMachine::updateDose() {
    if (m_currentState == MachineState::GRINDING) {
        m_brewParam.current_dose = max(m_brewParam.current_dose, m_unitGrinder.getCurrentAmount());
    }
}

void CoffeeMachine::updateBrewGroup() {
    m_brewGroupPosition = m_unitBrewGroup.getPosition();
    m_brewGroupCurrent = m_unitBrewGroup.getCurrent();
}

void CoffeeMachine::resetBrewParam() {
    m_brewParam.current_volume = 0.0f;
    m_brewParam.current_dose = 0.0f;
}



void CoffeeMachine::updateSensors() {
    updateTemperature();
    updateVolume();
    updateDose();
    updateBrewGroup();
}


void CoffeeMachine::printSensorValues() {
    Serial.print(" | TB_Temp : ");
    Serial.print(m_brewParam.current_temperature);
    Serial.print(" | GR_Amount : ");
    Serial.print(m_brewParam.current_dose);
    Serial.print(" | WC_Volume : ");
    Serial.print(m_brewParam.current_volume);
    Serial.print(" | BG_Position : ");
    Serial.print(m_brewGroupPosition);
    Serial.print(" | BG_Current : ");
    Serial.print(m_brewGroupCurrent);
    Serial.print(" | DutyCycle : ");
    Serial.print(m_unitThermoBlock.getDutyCycle());
    Serial.println();
}


// blocking
void CoffeeMachine::flush() {
    // 1. move down brewgroup
    m_unitBrewGroup.moveDown();
    while (m_unitBrewGroup.isMovingDown()) {
        m_unitBrewGroup.checkIfPressed();
    }

    // 2. switch on Pump for 5 seconds
    Serial.println("Flushing");
    unsigned long pump_start = millis();
    m_unitPump.switchOn();
    while (millis() - pump_start < 8000) {}
    m_unitPump.switchOff();
}


// ASSUMPTIONS: BREWGROUP IS HOMED AND OPEN, GRINDER FLAP IS HOMED AND CLOSED
void CoffeeMachine::makeCoffee() {
    if (m_newMenuID == LCD::makeCoffeeScreenID) {
        m_unitThermoBlock.update();

        switch (m_currentState)
        {
        case MachineState::IDLE:
            resetBrewParam();
            m_sdCard.setupBrewFile(&m_brewParam);
            m_unitThermoBlock.heatTo(m_brewParam.set_temperature, ThermoBlock::TBMode::PREBREW);
            m_currentState = MachineState::HEATING;
            break;

        case MachineState::HEATING:
            m_unitGrinder.setDose(m_brewParam.set_dose);
            m_unitGrinder.tareScale();
            m_unitGrinder.switchMotorOn();
            m_currentState = MachineState::GRINDING;
            break;

        case MachineState::GRINDING:
            m_unitGrinder.updateScale();

            if (m_unitGrinder.isFinished()) {
                m_unitGrinder.switchMotorOff();
                m_currentState = MachineState::BREWGROUP_INSERTION;
            }
            break;

        case MachineState::BREWGROUP_INSERTION:
            if (m_unitGrinder.isFinished()) {      
                m_unitGrinder.openFlap();
                m_grinderFlapOpenedTimestamp = millis();
                m_brewParam.current_dose = max(m_brewParam.current_dose, m_unitGrinder.getCurrentAmount());// update with last value
            }
            // wait for 2 seconds so that coffee ground can slide into the brewchamber
            if (m_unitGrinder.isOpen() && (millis() - m_grinderFlapOpenedTimestamp > 2000)) {
                m_unitBrewGroup.moveDown();
                m_unitPump.tareScale();
                m_currentState = MachineState::TAMPERING;
            }
            break;

        case MachineState::TAMPERING:
            if (m_unitBrewGroup.isMoving()) {
                m_unitBrewGroup.checkIfPressed();
            }

            if (m_unitBrewGroup.isPress() && m_unitThermoBlock.isSteadyState()) {
                m_unitPump.setAmount(m_brewParam.set_volume);
                m_brewParam.start_temperature = m_unitThermoBlock.getTemperature();
                m_unitThermoBlock.switchMode(ThermoBlock::TBMode::BREW);
                m_unitPump.switchOn();
                m_currentState = MachineState::PREINFUSION;
                m_preinfusionTimestamp = millis();

            }
            break;

        case MachineState::PREINFUSION:
            if (millis() - m_preinfusionTimestamp > 3000) {
                m_unitPump.switchOff();

                if (m_unitThermoBlock.isSteadyState()) {
                    m_unitPump.switchOn();
                    m_currentState = MachineState::EXTRACTION;
                }
            }
            break;

        case MachineState::EXTRACTION:
            if (m_unitPump.isFinished()) {
                m_unitPump.switchOff();

                m_unitThermoBlock.heatTo(config::TEMP_IDLE, ThermoBlock::TBMode::IDLE);
                m_unitGrinder.closeFlap();
                m_unitBrewGroup.moveUp();
                m_sdCard.close();

                m_currentState = MachineState::BREWGROUP_HOME;
            }
            break;

        case MachineState::BREWGROUP_HOME:
            // first home
            if (m_unitBrewGroup.isMovingUp()) {
                m_unitBrewGroup.checkIfHomed();
            }

            if (m_unitBrewGroup.isHome()) {
                m_unitBrewGroup.moveDown();
                m_currentState = MachineState::RETURN_TO_IDLE;
            }
            break;

        case MachineState::RETURN_TO_IDLE:
            if (m_unitBrewGroup.isMovingDown()) {
                m_unitBrewGroup.checkIfOpened();
            }

            if (m_unitBrewGroup.isOpen() && m_unitGrinder.isClose() && m_unitPump.isOff()) {
                m_newMenuID = 0;
                m_currentState = MachineState::IDLE;
                changeMenu();
            }

            if (m_SDCardEnabled) {
                m_sdCard.close();
            }
            break;
            resetBrewParam();

        }

        m_sdCard.writeFile(&m_brewParam);
    }
}




void CoffeeMachine::changeMenu() {
    if (m_currentMenuID != m_newMenuID) {
        switch (m_newMenuID) {
            case LCD::mainMenuID:
                LCD::drawMainMenu();
                break;
            case LCD::simpleCoffeeMenuID:
                LCD::drawSimpleCoffeeMenu();
                break;
            case LCD::customCoffeeMenuID:
                LCD::drawCustomMenu(&m_brewParam);
                break;
            case LCD::makeCoffeeScreenID:
                LCD::drawMakeCoffeeScreen(&m_brewParam);
                break;
            case LCD::controlMenuID:
                LCD::drawControlMenu();
                break;
            case LCD::thermoblockControlMenuID:
                LCD::drawThermoControlMenu();
                break;
            case LCD::stepResponseMenuID:
                LCD::drawStepResponseMenu(m_unitThermoBlock, m_unitPump);
                break;
            case LCD::grinderControlMenuID:
                //LCD::drawGrinderControlMenu(&m_grinderState, &m_setQuantity, &m_currentQuantity);
                break;
            case LCD::pumpControlMenuID:
                LCD::drawPumpControlMenu(m_unitPump);
                break;
        }

        m_currentMenuID = m_newMenuID;
    }
}



void CoffeeMachine::updateLCD() {
    switch (m_currentMenuID) {
        case LCD::mainMenuID:
            m_newMenuID = LCD::updateMainMenu();
            break;
        case LCD::simpleCoffeeMenuID:
            m_newMenuID = LCD::updateSimpleCoffeeMenu(&m_brewParam);
            break;
        case LCD::customCoffeeMenuID:
            m_newMenuID = LCD::updateCustomMenu(&m_brewParam);
            break;
        case LCD::makeCoffeeScreenID:
            m_newMenuID = LCD::updateMakeCoffeeScreen(&m_brewParam);
            makeCoffee();
            break;
        case LCD::controlMenuID:
            m_newMenuID = LCD::updateControlMenu();
            break;
        case LCD::thermoblockControlMenuID:
            m_newMenuID = LCD::updateThermoControlMenu();
            break;
        case LCD::stepResponseMenuID:
            m_newMenuID = LCD::updateStepResponseMenu(m_unitThermoBlock, m_unitPump);
            break;
        case LCD::grinderControlMenuID:
            //m_newMenuID = LCD::updateGrinderControlMenu(&m_grinderState, &m_setQuantity, &m_currentQuantity);
            break;
        case LCD::pumpControlMenuID:
            m_newMenuID = LCD::updatePumpControlMenu(m_unitPump);
            break;
    }

    changeMenu();
    
}


void CoffeeMachine::createStepResponse(double stepPower, bool waterFlow) {
    
    if (m_SDCardEnabled && !m_fileOpen) {
        char filename[12];
        generateStepFileName(filename);
        m_file = SD.open(filename, FILE_WRITE);
        m_fileOpen = true;
    }

    
    m_unitThermoBlock.power(stepPower);

    if (waterFlow) {
        m_unitPump.switchOn();
    }

    
    if (m_SDCardEnabled && m_fileOpen) {
        m_sdCard.writeFile(&m_brewParam);
        m_sdCard.close();
        m_fileOpen = false;
    }
}


void CoffeeMachine::returnToIdle() {
    if (m_currentState == MachineState::RETURN_TO_IDLE) {

        if (m_unitBrewGroup.getLastState() != BrewGroup::BGState::HOMED && !m_unitBrewGroup.isHome()) {
            m_unitBrewGroup.moveUp();
            m_unitBrewGroup.checkIfHomed();
        }

        if (m_unitBrewGroup.isHome()) {
            m_unitBrewGroup.moveDown();
        }
        // home Brewgroup
        // open Brewgroup
        // close Brewgroup Flap
        // set Temperature to IDLE
    }
}