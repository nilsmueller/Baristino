#include "utils.h"


// function to print a device address
void printDS18B20Address(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}


void printDS18B20AddressOCT(DeviceAddress deviceAddress)
{ 
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print("0x");
    if (deviceAddress[i] < 0x10) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (i < 7) Serial.print(", ");
  }
  Serial.println("");
}


void findDS18B20Sensors(const uint8_t busPin) {
    OneWire oneWire(busPin);
    DallasTemperature sensors(&oneWire);

    int numberOfDevices;
    DeviceAddress tempDeviceAddress;

    sensors.begin();
    numberOfDevices = sensors.getDeviceCount();

    // Loop through each device, print out address
    for(int i=0;i<numberOfDevices; i++) {
        // Search the wire for address
        if(sensors.getAddress(tempDeviceAddress, i)) {
            Serial.print("Found device ");
            Serial.print(i, DEC);
            Serial.print(" with address: ");
            printDS18B20Address(tempDeviceAddress);
            Serial.print(" | ");
            printDS18B20AddressOCT(tempDeviceAddress);
            Serial.println();
		} else {
		    Serial.print("Found ghost device at ");
		    Serial.print(i, DEC);
		    Serial.print(" but could not detect address. Check power and cabling");
		}
  }
}



void calibrateScale(const uint8_t pinDOUT, const uint8_t pinSCK, double refMass) {
  HX711 scale;
  scale.begin(pinDOUT, pinSCK);

  if (scale.is_ready()) {
    scale.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight [g] on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);


    Serial.print("Calibration Factor is [g] : ");
    Serial.println(reading / refMass * 1000);
  } 
  else {
    Serial.println("HX711 not found.");
  }

}



EggTimer::EggTimer() {
}


EggTimer::EggTimer(unsigned long duration_MS) : m_durationMS(duration_MS) {}


void EggTimer::setDurationMS(unsigned long duration_MS) {
  m_durationMS = duration_MS;
}


unsigned long EggTimer::getDurationMS() {
  return m_durationMS;
}


unsigned long EggTimer::getLeftDurationMS() {
  if (isActive()) {
    return m_startTimeMS + m_durationMS - millis();
  }
  else {
    return 0;
  }
}


void EggTimer::startTimer() {
  m_isActive = true;
}


void EggTimer::stopTimer() {
  m_isActive = false;
}


void EggTimer::resetTimer() {
    m_startTimeMS = millis();
}



bool EggTimer::isActive() {
  return m_isActive;
}


bool EggTimer::isFinished() {
  if (isActive()) {
    return (millis() - m_startTimeMS > m_durationMS) ? true : false;
  }
  return false;
}

void EggTimer::quickStart(unsigned long duration_MS) {
  stopTimer();
  setDurationMS(duration_MS);
  resetTimer();
  startTimer();
}

