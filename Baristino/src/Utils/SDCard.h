#pragma once

#include "Arduino.h"
#include <SD.h>
#include "utils.h"

class mySDCard {
public:
    mySDCard(uint8_t pinChipSelect);
    void initialize();
    bool isAvailable();
    void close();
    void getConfiguration(config::general *conf);
    //void updateConfiguration(config::general *conf);
    //void writeConfiguration(config::general *conf);

    void setupWarmupFile();
    void setupBrewFile(BrewParam *brewParameter);
    void writeFile(BrewParam *brewParameter);

private:
    File m_file;
    uint8_t m_pinCS;
    bool m_isAvailabe = false;
};


bool SD_available(const __FlashStringHelper * key);
int SD_findInt(const __FlashStringHelper * key);
float SD_findFloat(const __FlashStringHelper * key);
String SD_findString(const __FlashStringHelper * key);
int SD_findKey(const __FlashStringHelper * key, char * value);
int HELPER_ascii2Int(char *ascii, int length);
float HELPER_ascii2Float(char *ascii, int length);
String HELPER_ascii2String(char *ascii, int length);

uint8_t getNumOfFiles(char *dir);
void generateWarmupFileName(char *filename);
void generateBrewFileName(char *filename);
void generateStepFileName(char *filename);