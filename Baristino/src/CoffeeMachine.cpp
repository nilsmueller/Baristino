#include "CoffeeMachine.h"


// Constructor with member init
CoffeeMachine::CoffeeMachine() :
m_unitThermoBlock(pindef::SSR_THERMOBLOCK, pindef::DS18B20_THERMOBLOCK),
m_unitPump(pindef::SSR_WATERPUMP, pindef::BEVERAGE_LOADCELL_SCK, pindef::BEVERAGE_LOADCELL_DOUT),
m_unitGrinder(config::STEPPER_STEPS_PER_REV, pindef::STEPPER_1, pindef::STEPPER_2, pindef::STEPPER_3, pindef::STEPPER_4, pindef::HOPPER_LOADCELL_DOUT, pindef::HOPPER_LOADCELL_SCK, pindef::SSR_GRINDER),
m_unitBrewGroup(pindef::BREWGROUP_MOTOR_IN_1, pindef::BREWGROUP_MOTOR_IN_2, pindef::BREWGROUP_MOTOR_ENA, pindef::BREWGROUP_ENCODER_INPUT_A, pindef::BREWGROUP_ENCODER_INPUT_B, pindef::BREWGROUP_MOTOR_AMPMETER)
{
}


// initialization of peripheral devices
void CoffeeMachine::initialize() {
    LCD::initialize();
    m_unitThermoBlock.initialize();
    m_unitPump.initialize();
    m_unitGrinder.initialize();
    m_unitBrewGroup.initialize();

    
    if (SD.begin(pindef::CHIP_SELECT)) {
        m_SDCardEnabled = true;
        Serial.println("SD Card is present and ready.");
    }
    else {
        Serial.println("ERROR : SD Card can not be found");
    }

    //LCD::drawMainMenu();
}



void CoffeeMachine::warmup() {

    LCD::drawWarmUpScreen();
    
    m_unitThermoBlock.setTemperature(config::TEMP_IDLE);
    m_unitThermoBlock.switchMode(ThermoBlock::TBMode::WARMUP);
    m_unitThermoBlock.startPIDControl();

    if (m_SDCardEnabled) {
        char filename[17];
        generateWarmupFileName(filename);
        Serial.println(filename);
        m_file = SD.open(filename, FILE_WRITE);
        m_file.println("time,temp,PIDsp,PIDpv,duty,qty,vol");
    }

    double progress;    
    while (!m_unitThermoBlock.isSteadyState()) {
        m_unitThermoBlock.update();
        m_brewParam.current_temperature = m_unitThermoBlock.getTemperature();
        progress = (config::TEMP_IDLE - abs(config::TEMP_IDLE - m_brewParam.current_temperature)) / config::TEMP_IDLE;
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

        writeToFile();
        delay(10);
    }

    if (m_SDCardEnabled) {
        m_file.close();
    }
    
    // home brew group
    Serial.println("BREWGROUP_HOMING ");
    m_unitBrewGroup.moveUp();
    while (m_unitBrewGroup.isMovingUp()) {
        //Serial.println(m_unitBrewGroup.getCurrent());
        m_unitBrewGroup.checkIfHomed();
    }

    delay(1000);
    Serial.println("BREWGROUP_OPENING");
    // open brew group
    m_unitBrewGroup.moveDown();
    while (m_unitBrewGroup.isMovingDown()) {
        //Serial.print(m_unitBrewGroup.getCurrent());
        //Serial.println(m_unitBrewGroup.getPosition());
        m_unitBrewGroup.checkIfOpened();
    }

    LCD::drawMainMenu();
}


void CoffeeMachine::updateTemperature() {
    m_unitThermoBlock.update();
    m_brewParam.current_temperature = m_unitThermoBlock.getTemperature();
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
        //m_unitGrinder.updateScale();
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


// ASSUMPTIONS: BREWGROUP IS HOMED AND OPEN, GRINDER FLAS IS HOMED AND CLOSED
void CoffeeMachine::makeCoffee() {
    if (m_newMenuID == 21) {
        m_unitThermoBlock.update();


        switch (m_currentState)
        {
        case MachineState::IDLE:
            // SD TEST 
            resetBrewParam();

            if (m_SDCardEnabled) {
                char filename[17];
                generateBrewFileName(filename);
                Serial.println(filename);
                m_file = SD.open(filename, FILE_WRITE);
                writeHeader(&m_file, &m_brewParam);
                m_file.println("time,temp,PIDsp,PIDpv,duty,qty,vol,Kp,Ki,Kd");
            }
            m_unitThermoBlock.setTemperature(m_brewParam.set_temperature);
            m_unitThermoBlock.startPIDControl();
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
            // LOGICAL ERROR HERE
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
                //m_unitThermoBlock.stopPIDControl();
                //m_unitThermoBlock.powerOn();
                m_brewParam.start_temperature = m_unitThermoBlock.getTemperature();
                m_unitThermoBlock.switchMode(ThermoBlock::TBMode::BREW);
                //m_unitThermoBlock.startPIDControl();
                //
                m_unitPump.switchOn();
                m_currentState = MachineState::EXTRACTION;

            }
            break;

        case MachineState::EXTRACTION:
            if (m_unitPump.isFinished()) {
                m_unitPump.switchOff();
                //
                m_unitThermoBlock.powerOff();
                //
                m_unitGrinder.closeFlap();
                m_unitThermoBlock.stopPIDControl();
                m_unitBrewGroup.moveUp();

                    if (m_SDCardEnabled) {
                        m_file.close();
                    }

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
                m_file.close();
            }
            break;
            resetBrewParam();

        default:
            break;
        }

        writeToFile();
    }
}




void CoffeeMachine::changeMenu() {
    if (m_currentMenuID != m_newMenuID) {
        switch (m_newMenuID) {
            case 0:
                LCD::drawMainMenu();
                break;
            case 1:
                LCD::drawSimpleCoffeeMenu();
                break;
            case 2:
                LCD::drawCustomMenu(&m_brewParam);
                break;
            case 21:
                LCD::drawMakeCoffeeScreen(&m_brewParam);
                break;
            case 3:
                LCD::drawControlMenu();
                break;
            case 31:
                LCD::drawThermoControlMenu();
                break;
            case 38:
                LCD::drawStepResponseMenu(m_unitThermoBlock, m_unitPump);
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
            m_newMenuID = LCD::updateSimpleCoffeeMenu(&m_brewParam);
            break;
        case 2:
            m_newMenuID = LCD::updateCustomMenu(&m_brewParam);
            break;
        case 21:
            m_newMenuID = LCD::updateMakeCoffeeScreen(&m_brewParam);
            makeCoffee();
            break;
        case 3:
            m_newMenuID = LCD::updateControlMenu();
            break;
        case 31:
            m_newMenuID = LCD::updateThermoControlMenu();
            break;
        case 38:
            m_newMenuID = LCD::updateStepResponseMenu(m_unitThermoBlock, m_unitPump);
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


void CoffeeMachine::writeToFile() {
    if (m_SDCardEnabled) {
        if (m_file) {
            m_file.print(millis());
            m_file.print(",");
            m_file.print(m_brewParam.current_temperature);
            m_file.print(",");
            m_file.print(m_unitThermoBlock.getPIDsetpoint());
            m_file.print(",");
            m_file.print(m_unitThermoBlock.getPIDinput());
            m_file.print(",");
            m_file.print(m_unitThermoBlock.getDutyCycle());
            m_file.print(",");
            m_file.print(m_brewParam.current_dose);
            m_file.print(",");
            m_file.print(m_brewParam.current_volume);
            m_file.print(",");
            m_file.print(m_unitThermoBlock.getPIDKp());
            m_file.print(",");
            m_file.print(m_unitThermoBlock.getPIDKi());
            m_file.print(",");
            m_file.print(m_unitThermoBlock.getPIDKd());
            m_file.println();
        }
        else {
            Serial.println("Error opening File");
        }
    }
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
        writeToFile();
        m_file.close();
        m_fileOpen = false;
    }
}