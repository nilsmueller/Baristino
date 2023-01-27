#include "CoffeeMachine.h"


// Constructor with member init
CoffeeMachine::CoffeeMachine() :
m_unitThermoBlock(pindef::SSR_THERMOBLOCK, pindef::DS18B20_THERMOBLOCK),
m_unitPump(pindef::SSR_WATERPUMP, pindef::BEVERAGE_LOADCELL_SCK, pindef::BEVERAGE_LOADCELL_DOUT),
m_unitGrinder(config::STEPPER_STEPS_PER_REV, pindef::STEPPER_1, pindef::STEPPER_2, pindef::STEPPER_3, pindef::STEPPER_4, pindef::HOPPER_LOADCELL_DOUT, pindef::HOPPER_LOADCELL_SCK, pindef::SSR_GRINDER),
m_unitBrewGroup(pindef::BREWGROUP_MOTOR_IN_1, pindef::BREWGROUP_MOTOR_IN_2, pindef::BREWGROUP_MOTOR_ENA, pindef::BREWGROUP_ENCODER_INPUT_A, pindef::BREWGROUP_ENCODER_INPUT_B, pindef::BREWGROUP_MOTOR_AMPMETER)
{
    Serial.println("Hi i am the Constructor");
}


// initialization of peripheral devices
void CoffeeMachine::initialize() {
    LCD::initialize();
    m_unitThermoBlock.initialize();
    m_unitPump.initialize();
    m_unitGrinder.initialize();
    m_unitBrewGroup.initialize();

    LCD::drawMainMenu();
}


void CoffeeMachine::warmup() {

    LCD::drawWarmUpScreen();
    m_unitThermoBlock.setTemperature(config::TEMP_WARMUP);
    m_unitThermoBlock.startPIDControl();

    
    while (!m_unitThermoBlock.isSteadyState()) {
        m_unitThermoBlock.update();
        m_currentTemperature = m_unitThermoBlock.getTemperature();

        LCD::updateWarmUpScreen(m_currentTemperature / config::TEMP_WARMUP);
        Serial.print("WARMUP - Temp = ");
        Serial.print(m_currentTemperature);
        Serial.print(" / ");
        Serial.print(config::TEMP_WARMUP);
        Serial.print(" [°C] ");
        m_unitThermoBlock.watchPIDControl();
        Serial.println();
        delay(10);
    }

    // home brew group
    Serial.println("BREWGROUP_HOMING ");
    m_unitBrewGroup.moveUp();
    while (m_unitBrewGroup.isMovingUp()) {
        m_unitBrewGroup.updatePosition();
        m_unitBrewGroup.checkIfHomed();
    }
    delay(4000);

    Serial.println("BREWGORUP_OPENING");
    // open brew group
    m_unitBrewGroup.moveDown();
    while (m_unitBrewGroup.isMovingDown()) {
        m_unitBrewGroup.updatePosition();
        m_unitBrewGroup.checkIfOpened();
    }

    LCD::drawMainMenu();
}


void CoffeeMachine::updateSensors() {
    m_unitPump.updateScale();
    m_currentVolume = m_unitPump.getCurrentAmount();

    m_unitGrinder.updateScale();
    m_currentQuantity = m_unitGrinder.getCurrentAmount();

    m_unitThermoBlock.update();
    m_currentTemperature = m_unitThermoBlock.getTemperature();

    m_unitBrewGroup.updatePosition();
    m_brewGroupPosition = m_unitBrewGroup.getPosition();
    m_brewGroupCurrent = m_unitBrewGroup.getCurrent();
}


void CoffeeMachine::printSensorValues() {
    Serial.print("Temp : ");
    Serial.print(m_currentTemperature);
    Serial.print(", Amount : ");
    Serial.print(m_currentQuantity);
    Serial.print(", Volume : ");
    Serial.print(m_currentVolume);
    Serial.print("   BG_Position : ");
    Serial.print(m_brewGroupPosition);
    Serial.print("   BG_Current : ");
    Serial.print(m_brewGroupCurrent);
    Serial.println();
}


// ASSUMPTIONS: BREWGROUP IS HOMED AND OPEN, GRINDER FLAS IS HOMED AND CLOSED
void CoffeeMachine::makeCoffee() {
    /*
    // check the necessary pre-conditions
    if(!m_unitPump.isOff() || !m_unitBrewGroup.isOpen() || !m_unitGrinder.isClose() ){
        m_unitPump.switchOff();
        // LOGICAL ERROR HERE
        m_unitBrewGroup.open();
        m_unitGrinder.closeFlap();
    }
    */

    m_unitThermoBlock.update();

    switch (m_currentState)
    {
    case MachineState::IDLE:
        m_unitThermoBlock.setTemperature(m_setTemperature);
        m_unitThermoBlock.startPIDControl();
        m_currentState = MachineState::HEATING;
        break;

    case MachineState::HEATING:
        Serial.print("HEATING ");
        m_unitGrinder.setDose(m_setQuantity);
        m_unitGrinder.tareScale();
        m_unitGrinder.switchMotorOn();
        m_currentState = MachineState::GRINDING;
        break;

    case MachineState::GRINDING:
        Serial.print("GRINDING ");
        m_unitGrinder.updateScale();

        if (m_unitGrinder.isFinished()) {
            m_unitGrinder.switchMotorOff();
            m_currentState = MachineState::BREWGROUP_INSERTION;
        }
        break;

    case MachineState::BREWGROUP_INSERTION:
        Serial.print("BREWGROUP_INSERTION ");
        // LOGICAL ERROR HERE
        if (m_unitGrinder.isFinished()) {      
            m_unitGrinder.openFlap();
            m_grinderFlapOpenedTimestamp = millis();
        }
        // wait for 2 seconds so that coffee ground can slide into the brewchamber
        if (m_unitGrinder.isOpen() && (millis() - m_grinderFlapOpenedTimestamp > 2000)) {
            m_unitBrewGroup.moveDown();
            m_currentState = MachineState::TAMPERING;
        }
        break;

    case MachineState::TAMPERING:
        Serial.print("TAMPERING ");
        if (m_unitBrewGroup.isMoving()) {
            m_unitBrewGroup.updatePosition();
            m_unitBrewGroup.checkIfPressed();
        }

        if (m_unitBrewGroup.isPress() && m_unitThermoBlock.isSteadyState()) {
            m_unitPump.setAmount(m_setVolume);
            m_unitPump.tareScale();
            m_unitPump.switchOn();
            m_currentState = MachineState::EXTRACTION;
        }
        break;
    
    case MachineState::EXTRACTION:
        Serial.print("EXTRACTION ");
        if (m_unitPump.isFinished()) {
            m_unitPump.switchOff();
            m_unitGrinder.closeFlap();
            m_unitThermoBlock.stopPIDControl();
            m_unitBrewGroup.moveUp();
            m_currentState = MachineState::BREWGROUP_HOME;
        }
        else {
            m_unitPump.updateScale();
        }
        break;

    case MachineState::BREWGROUP_HOME:
        Serial.print("BREWGROUP_HOME ");
        // first home
        if (m_unitBrewGroup.isMovingUp()) {
            m_unitBrewGroup.updatePosition();
            m_unitBrewGroup.checkIfHomed();
        }

        if (m_unitBrewGroup.isHome()) {
            m_unitBrewGroup.moveDown();
            m_currentState = MachineState::RETURN_TO_IDLE;
        }
        break;

    case MachineState::RETURN_TO_IDLE:
        Serial.print("RETURN_TO_IDLE ");

        if (m_unitBrewGroup.isMovingDown()) {
            m_unitBrewGroup.updatePosition();
            m_unitBrewGroup.checkIfOpened();
        }

        if (m_unitBrewGroup.isOpen() && m_unitGrinder.isClose() && m_unitPump.isOff()) {
            m_currentMenuID = 1;
            m_currentState = MachineState::IDLE;
        }
        break;

    default:
        break;
    }


}





void CoffeeMachine::changeMenu() {
    if (m_currentMenuID != m_newMenuID) {
        switch (m_newMenuID) {
            case 0:
                LCD::drawMainMenu();
                //m_process = Process::NONE;
                break;
            case 1:
                LCD::drawCoffeeMenu(&m_setVolume, &m_setTemperature, &m_setQuantity);
                break;
            case 11:
                break;
            case 3:
                LCD::drawControlMenu();
                break;
            case 33:
                //LCD::drawGrinderControlMenu(&m_grinderState, &m_setQuantity, &m_currentQuantity);
                break;
            case 34:
                LCD::drawPumpControlMenu(m_unitPump);
                break;
            default:
                //LCD::drawMainMenu();
                //m_process = Process::NONE;
                break;
        }

        m_currentMenuID = m_newMenuID;
    }
}



void CoffeeMachine::updateLCD() {
    switch (m_currentMenuID) {
        case 0:
            m_newMenuID = LCD::updateMainMenu();
            break;
        case 1:
            m_newMenuID = LCD::updateCoffeeMenu(&m_setVolume, &m_setTemperature, &m_setQuantity);
            break;
        case 11:
            makeCoffee();
            break;
        case 3:
            m_newMenuID = LCD::updateControlMenu();
            break;
        case 33:
            //m_newMenuID = LCD::updateGrinderControlMenu(&m_grinderState, &m_setQuantity, &m_currentQuantity);
            break;
        case 34:
            m_newMenuID = LCD::updatePumpControlMenu(m_unitPump);
            break;
        default:
            //m_newMenuID = LCD::updateMainMenu();
            break;
    }

    changeMenu();
    
}


