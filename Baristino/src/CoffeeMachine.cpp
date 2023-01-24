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

    m_unitThermoBlock.initialize();
    m_unitPump.initialize();
    m_unitGrinder.initialize();

    //LCD::initialize();
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
        Serial.println(" [°C]");
        delay(100);
    }

    // home and open brew group
}


void CoffeeMachine::updateSensors() {
    m_unitPump.updateScale();
    m_currentVolume = m_unitPump.getCurrentAmount();

    m_unitGrinder.updateScale();
    m_currentQuantity = m_unitGrinder.getCurrentAmount();

    m_unitThermoBlock.update();
    m_currentTemperature = m_unitThermoBlock.getTemperature();
}


// ASSUMPTIONS: BREWGROUP IS HOMED AND OPEN, GRINDER FLAS IS HOMED AND CLOSED
void CoffeeMachine::makeCoffeeNonBlocking() {
    if (m_brewState) {
        // check the necessary pre-conditions
        if(!m_unitPump.isOff() || !m_unitBrewGroup.isOpen() || !m_unitGrinder.isClose() ){
            m_unitPump.switchOff();
            // LOGICAL ERROR HERE
            m_unitBrewGroup.open();
            m_unitGrinder.closeFlap();
        }

        m_unitThermoBlock.update();

        switch (m_currentState)
        {
        case MachineState::IDLE:
            m_unitThermoBlock.setTemperature(m_setTemperature);
            m_unitThermoBlock.startPIDControl();
            m_currentState = MachineState::HEATING;
            break;

        case MachineState::HEATING:
            m_unitGrinder.setDose(m_setQuantity);
            m_unitGrinder.tareScale();
            m_unitGrinder.switchMotorOn();
            m_currentState = MachineState::GRINDING;
            break;

        case MachineState::GRINDING:
            m_unitGrinder.updateScale();
            if (m_unitGrinder.isFinished()) {
                m_currentState = MachineState::BREWGROUP_INSERTION;
            }
            break;

        case MachineState::BREWGROUP_INSERTION:
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
            if (m_unitBrewGroup.isMoving()) {
                m_unitBrewGroup.checkIfPressed();
            }

            if (m_unitBrewGroup.isPress()) {
                m_unitPump.setAmount(m_setVolume);
                m_unitPump.tareScale();
                m_unitPump.switchOn();
                m_currentState = MachineState::EXTRACTION;
            }
            break;
        
        case MachineState::EXTRACTION:
            if (m_unitPump.isFinished()) {
                m_unitPump.switchOff();
                m_currentState = MachineState::RETURN_TO_IDLE;
            }
            else {
                m_unitPump.updateScale();
            }

        case MachineState::RETURN_TO_IDLE:
            m_unitGrinder.closeFlap();
            m_unitBrewGroup.moveUp();
            m_unitThermoBlock.stopPIDControl();
            m_unitBrewGroup.updatePosition();

            // first home
            if (m_unitBrewGroup.isMovingUp()) {
                m_unitBrewGroup.checkIfHomed();
            }

            // then open
            if (m_unitBrewGroup.isHome()) {
                m_unitBrewGroup.moveDown();
            }

            if (m_unitBrewGroup.isMovingDown()) {
                m_unitBrewGroup.checkIfOpened();
            }

            if (m_unitBrewGroup.isOpen() && m_unitGrinder.isClose() && m_unitPump.isOff()) {
                m_brewState = false;
                m_currentState = MachineState::IDLE;
            }
            break;

        default:
            break;
        }


    }
}


void CoffeeMachine::makeCoffee() {

    // TEMPERATURE CONTROL
    /**/
    m_unitThermoBlock.setTemperature(m_setTemperature);
    delay(500);

    m_unitThermoBlock.startPIDControl();
    delay(500);

    while (!m_unitThermoBlock.isSteadyState() )
    {
         m_unitThermoBlock.update();

        Serial.print("Temperature : ");
        Serial.print(m_unitThermoBlock.getTemperature());
        Serial.print(" / ");
        Serial.print(m_setTemperature);
        Serial.print(" [°C]");
        Serial.print(" isHeating : ");
        Serial.print(m_unitThermoBlock.isHeating());
        m_unitThermoBlock.watchPIDControl();
        Serial.println();

        // UPDATE LCD
    }

    
    Serial.print("Starting to Home");
    delay(500);
    m_unitBrewGroup.home();


    Serial.print("Starting to Open");
    delay(500);
    m_unitBrewGroup.open();
    

    Serial.print("Starting to grind");
    delay(500);
    m_unitGrinder.grind(m_setQuantity);

    delay(500);
    Serial.print("Starting to tamper");
    m_unitBrewGroup.press();


    delay(2000);
    Serial.print("Starting to Pump");
    m_unitPump.pumpAmount(m_setVolume);

    Serial.print("Stopping Heating Process");
    m_unitThermoBlock.stopPIDControl();

    delay(500);
    Serial.print("Starting to Home");
    m_unitBrewGroup.home();

    delay(500);
    Serial.print("Starting to Open");
    m_unitBrewGroup.open();
    
}

/*
void CoffeeMachine::updatePump() {
    if(m_pumpState == PumpState::OFF) {
        m_unitWaterPump.switchOff();
    }
    else if (m_pumpState == PumpState::ON)
    {
        m_unitWaterPump.switchOn();
        // DEGBUG
        m_currentVolume = m_unit.ge
        delay(100);
    }
    else if (m_pumpState == PumpState::AUTO) {
        if (m_currentVolume <= m_setVolume) {
            m_unitWaterPump.switchOn();
            // DEGBUG
            m_currentVolume += 1.213;
            delay(100);
        }
        else {
            m_unitWaterPump.switchOff();
            m_pumpState = PumpState::OFF;
        }
    }
    
}


void CoffeeMachine::updateGrinder() {
    if(m_grinderState == GrinderState::OFF) {
        m_unitGrinder.switchOff();
    }
    else if (m_grinderState == GrinderState::ON)
    {
        m_unitGrinder.switchOn();
        // DEGBUG
        m_currentQuantity += 0.13;
        delay(100);
    }
    else if (m_grinderState == GrinderState::AUTO) {
        if (m_currentQuantity <= m_setQuantity) {
            m_unitGrinder.switchOn();
            // DEGBUG
            m_currentQuantity += 0.13;
            delay(100);
        }
        else {
            m_unitGrinder.switchOff();
            m_grinderState = GrinderState::OFF;
        }
    }
    
}


void CoffeeMachine::update() {
    m_unitThermoBlock.update();

    
    //  sleep if lcd has not been touched
    if (LCD::getTouchCoord()) {
        m_lastTouch = millis();
    }

    if ((millis() - m_lastTouch) > config::SLEEP_AFTER) {
        LCD::sleep();
    }
    


    if(!isError()) {
        updateDisplay();
        updatePump();
        updateGrinder();
    }
}


void CoffeeMachine::changeMenu() {
    if (m_currentMenuID != m_newMenuID) {
        switch (m_newMenuID) {
            case 0:
                LCD::drawMainMenu();
                break;
            case 1:
                LCD::drawCoffeeMenu(&m_setVolume, &m_setTemperature, &m_setQuantity);
                break;
            case 3:
                LCD::drawControlMenu();
                break;
            case 33:
                LCD::drawGrinderControlMenu(&m_grinderState, &m_setQuantity, &m_currentQuantity);
                break;
            case 34:
                LCD::drawPumpControlMenu(&m_pumpState, &m_setVolume, &m_currentVolume);
                break;
            default:
                LCD::drawMainMenu();
                break;
        }

        m_currentMenuID = m_newMenuID;
    }
}

void CoffeeMachine::updateDisplay() {
    switch (m_currentMenuID) {
        case 0:
            m_newMenuID = LCD::updateMainMenu();
            break;
        case 1:
            m_newMenuID = LCD::updateCoffeeMenu(&m_setVolume, &m_setTemperature, &m_setQuantity);
            break;
        case 3:
            m_newMenuID = LCD::updateControlMenu();
            break;
        case 33:
            m_newMenuID = LCD::updateGrinderControlMenu(&m_grinderState, &m_setQuantity, &m_currentQuantity);
            break;
        case 34:
            m_newMenuID = LCD::updatePumpControlMenu(&m_pumpState, &m_setVolume, &m_currentVolume);
            break;
        default:
            m_newMenuID = LCD::updateMainMenu();
            break;
    }

    changeMenu();
    
}

*/

