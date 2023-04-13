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
  if (isActive()) {
    m_startTimeMS = millis();
  }
}



bool EggTimer::isActive() {
  return m_isActive;
}


bool EggTimer::isFinished() {
  if (isActive()) {
    return (millis() - m_startTimeMS > m_durationMS) ? true : false;
  }
}

