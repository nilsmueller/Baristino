#include "SDCard.h"


#define KEY_MAX_LENGTH    30 // change it if key is longer
#define VALUE_MAX_LENGTH  30 // change it if value is longer



mySDCard::mySDCard(uint8_t pinChipSelect) : m_pinCS(pinChipSelect) {
}


void mySDCard::initialize() {
  if (SD.begin(m_pinCS)) {
    m_isAvailabe = true;
    Serial.println("SD Card is present and ready.");
  }
  else {
    Serial.println("ERROR : SD Card can not be found");
  }
}


bool mySDCard::isAvailable() {
  return m_isAvailabe;
}


void mySDCard::getConfiguration(config::general *conf) {
  if (isAvailable()) {
    conf->num_beverages = SD_findInt(F("num_beverages"));
    conf->sum_quantity = SD_findFloat(F("sum_quantity"));
    conf->sum_volume = SD_findFloat(F("sum_volume"));
    conf->set_tmp = SD_findFloat(F("set_tmp"));
    conf->set_vol = SD_findFloat(F("set_vol"));
    conf->set_qty = SD_findFloat(F("set_qty"));
    conf->set_preinf = SD_findFloat(F("set_preinf"));
    
  }
}




void mySDCard::setupWarmupFile() {
  if (isAvailable()) {
    char filename[17];
    generateWarmupFileName(filename);
    m_file = SD.open(filename, FILE_WRITE);
    m_file.println("time,temperature,set_temperature,duty_cycle,quantity,volume");
  }
}

void mySDCard::setupBrewFile(BrewParam *brewParameter) {
  if (isAvailable()) {
    char filename[17];
    generateBrewFileName(filename);
    m_file = SD.open(filename, FILE_WRITE);

    // write header
    m_file.print("set_temperature,");
    m_file.println(brewParameter->set_temperature);
    m_file.print("set_volume,");
    m_file.println(brewParameter->set_volume);
    m_file.print("set_dose,");
    m_file.println(brewParameter->set_dose);

    // write column names
    m_file.println("time,temperature,set_temperature,duty_cycle,quantity,volume");
  }
}



void mySDCard::writeFile(BrewParam *brewParameter) {
  if (isAvailable()) {
    if (m_file) {
      m_file.print(millis());
      m_file.print(",");
      m_file.print(brewParameter->current_temperature);
      m_file.print(",");
      m_file.print(brewParameter->set_temperature);
      m_file.print(",");
      m_file.print(brewParameter->current_dutyCycle);
      m_file.print(",");
      m_file.print(brewParameter->current_dose);
      m_file.print(",");
      m_file.print(brewParameter->current_volume);
      m_file.println();
    }
    else {
      Serial.println("Error opening File");
    }
  }
}


void mySDCard::close() {
  if (isAvailable() && m_file) {
    m_file.close();
  }
}






bool SD_available(const __FlashStringHelper * key) {
  char value_string[VALUE_MAX_LENGTH];
  int value_length = SD_findKey(key, value_string);
  return value_length > 0;
}

int SD_findInt(const __FlashStringHelper * key) {
  char value_string[VALUE_MAX_LENGTH];
  int value_length = SD_findKey(key, value_string);
  return HELPER_ascii2Int(value_string, value_length);
}

float SD_findFloat(const __FlashStringHelper * key) {
  char value_string[VALUE_MAX_LENGTH];
  int value_length = SD_findKey(key, value_string);
  return HELPER_ascii2Float(value_string, value_length);
}

String SD_findString(const __FlashStringHelper * key) {
  char value_string[VALUE_MAX_LENGTH];
  int value_length = SD_findKey(key, value_string);
  return HELPER_ascii2String(value_string, value_length);
}

int SD_findKey(const __FlashStringHelper * key, char * value) {
  File configFile = SD.open("config.txt");

  if (!configFile) {
    Serial.print(F("SD Card: error on opening file "));
    Serial.println("config.txt");
    return 0 ;
  }

  char key_string[KEY_MAX_LENGTH];
  char SD_buffer[KEY_MAX_LENGTH + VALUE_MAX_LENGTH + 1]; // 1 is = character
  int key_length = 0;
  int value_length = 0;

  // Flash string to string
  PGM_P keyPoiter;
  keyPoiter = reinterpret_cast<PGM_P>(key);
  byte ch;
  do {
    ch = pgm_read_byte(keyPoiter++);
    if (ch != 0)
      key_string[key_length++] = ch;
  } while (ch != 0);

  // check line by line
  while (configFile.available()) {
    int buffer_length = configFile.readBytesUntil('\n', SD_buffer, 100);
    if (SD_buffer[buffer_length - 1] == '\r')
      buffer_length--; // trim the \r

    if (buffer_length > (key_length + 1)) { // 1 is = character
      if (memcmp(SD_buffer, key_string, key_length) == 0) { // equal
        if (SD_buffer[key_length] == '=') {
          value_length = buffer_length - key_length - 1;
          memcpy(value, SD_buffer + key_length + 1, value_length);
          break;
        }
      }
    }
  }

  configFile.close();  // close the file
  return value_length;
}

int HELPER_ascii2Int(char *ascii, int length) {
  int sign = 1;
  int number = 0;

  for (int i = 0; i < length; i++) {
    char c = *(ascii + i);
    if (i == 0 && c == '-')
      sign = -1;
    else {
      if (c >= '0' && c <= '9')
        number = number * 10 + (c - '0');
    }
  }

  return number * sign;
}

float HELPER_ascii2Float(char *ascii, int length) {
  int sign = 1;
  int decimalPlace = 0;
  float number  = 0;
  float decimal = 0;

  for (int i = 0; i < length; i++) {
    char c = *(ascii + i);
    if (i == 0 && c == '-')
      sign = -1;
    else {
      if (c == '.')
        decimalPlace = 1;
      else if (c >= '0' && c <= '9') {
        if (!decimalPlace)
          number = number * 10 + (c - '0');
        else {
          decimal += ((float)(c - '0') / pow(10.0, decimalPlace));
          decimalPlace++;
        }
      }
    }
  }

  return (number + decimal) * sign;
}

String HELPER_ascii2String(char *ascii, int length) {
  String str;
  str.reserve(length);
  str = "";

  for (int i = 0; i < length; i++) {
    char c = *(ascii + i);
    str += String(c);
  }

  return str;
}




uint8_t getNumOfFiles(char *dir) {
    uint8_t num_files = 0;

    File root = SD.open(dir);

    while (true) {
        File entry =  root.openNextFile();
        
        if (! entry) {
        // no more files
        break;
        }
        num_files++;
    }
    return num_files;
}





void generateWarmupFileName(char *filename) {
  int number = getNumOfFiles("WARMUP") + 1;
  sprintf(filename, "WARMUP/%04d.txt", number);
}


void generateBrewFileName(char *filename) {
  int number = getNumOfFiles("BREW") + 1;
  sprintf(filename, "BREW/%04d.txt", number);
}


void generateStepFileName(char *filename) {
  int number = getNumOfFiles("STEP") + 1;
  sprintf(filename, "STEP/%04d.txt", number);
}


