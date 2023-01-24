#ifndef _LCD_H
#define _LCD_H

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif

#include "../configuration.h"
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>


// TOUCH CONFIGURATIONS
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// COLORS
#define myBLACK   0x2946
#define myBLUE    0x24B7
#define myDARKBLUE 0x000E
#define myRED     0xC9A6
#define myGREEN   0x1F6F
#define myMAGENTA 0xF81F
#define myYELLOW  0xEE23
#define myPINK    0xE8F2
#define myORANGE  0xEB83
#define myWHITE   0xFFFF

// LCD MENU IDs
// |--- "0" : Main Menu
// |
// |--- "1" : Coffee Menu
// |     |--- "11" : Coffee Brew Menu
// |
// |--- "2" : Profile Menu
// |
// |--- "3" : Control Menu
// |     |--- "31" : ThermoBlock Control Menu
// |     |--- "32" : BrewGroup Control Menu
// |     |--- "33" : Grinder Control Menu
// |     |--- "34" : Pump Control Menu  
// |
// |--- "4" : Settings Menu

namespace LCD {
// Frame
extern const int LCD_WIDTH;
extern const int LCD_HEIGHT;
extern const int LCD_PAD;
extern const int LCD_BORDER;

// TopBar
extern const int LCD_TOPBAR_WIDTH;
extern const int LCD_TOPBAR_HEIGHT;
extern const int LCD_TOPBAR_OFFSET_X;
extern const int LCD_TOPBAR_OFFSET_Y;

// MainBody
extern const int LCD_MAIN_ORIGIN_X;
extern const int LCD_MAIN_ORIGIN_Y;
extern const int LCD_MAIN_WIDTH;
extern const int LCD_MAIN_HEIGHT;

extern int touch_pixel_x, touch_pixel_y;

extern uint8_t controlMenuID;
extern uint8_t pumpControlMenuID;
extern uint8_t grinderControlMenuID;

extern MCUFRIEND_kbv tft;

extern void initialize();

extern bool getTouchCoord();
extern void sleep();
extern void drawWarmUpScreen();
extern void drawEmptyScreen();
extern void drawEmptyScreenDouble();
extern void drawMainMenu();
extern void drawCoffeeMenu(double *volume, double *temperature, double *quantity);
extern void drawControlMenu();
extern void drawPumpControlMenu(PumpState *pumpStatus, double *setVolume, double *currentVolume);
extern void drawGrinderControlMenu(GrinderState *grinderStatus, double *setQuantity, double *currentQuantity);
extern void drawErrorScreen(ErrorCode code);

extern void updateWarmUpScreen(double percentage);
extern unsigned int updateMainMenu();
extern unsigned int updateCoffeeMenu(double *volume, double *temperature, double *quantity);
extern unsigned int updateControlMenu();
extern unsigned int updatePumpControlMenu(PumpState *pumpStatus, double *currentVolume, double *setVolume);
extern unsigned int updateGrinderControlMenu(GrinderState *grinderStatus, double *currentQuantity, double *setQuantity);

}

#endif