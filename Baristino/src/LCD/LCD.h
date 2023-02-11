#ifndef _LCD_H
#define _LCD_H

#if (ARDUINO > 100)
#include "Arduino.h"
#else
#include "WProgramm.h"
#endif


#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>

//#include "../configuration.h"
#include "../CoffeeMachine.h"
#include "../Peripherals/Pump.h"
#include "../Peripherals/ThermoBlock.h"

// TOUCH CONFIGURATIONS
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// COLORS
#define myBLACK   0x2946
#define myBLUE    0x24B7 // 33, 150, 190
#define myDARKBLUE 0x000E // 0, 0, 115
#define myDARKBLUE2 0x1BD3 // 27, 120, 152
#define myRED     0xC9A6
#define myGREEN   0x1F6F // 30, 236, 120
#define myDARKGREEN 0x0C68 // 12, 141, 68
#define myMAGENTA 0xF81F // 
#define myYELLOW  0xEE23 // 232, 169, 30
#define myDARKYELLOW 0xD562 // 208, 174, 22
#define myPINK    0xE8F2 
#define myORANGE  0xEB83 // 232, 112, 30
#define myDARKORANGE 0xBAC2 // 189, 88, 20
#define myWHITE   0xFFFF

// LCD MENU IDs
// |--- "0" : Main Menu
// |
// |--- "1" : Coffee Menu
// |     |--- "11" : Make Coffee Menu
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

extern uint16_t controlMenuID;
extern uint16_t pumpControlMenuID;
extern uint16_t grinderControlMenuID;
extern uint16_t thermoblockControlMenuID;
extern uint16_t stepResponseMenuID;

extern MCUFRIEND_kbv tft;

extern void initialize();

extern bool getTouchCoord();
extern void sleep();
extern void drawWarmUpScreen();
extern void drawEmptyScreen();
extern void drawEmptyScreenDouble();
extern void drawMainMenu();
extern void drawCoffeeMenu(double *volume, double *temperature, double *quantity);
extern void drawMakeCoffeeScreen();
extern void drawControlMenu();
extern void drawThermoControlMenu();
extern void drawStepResponseMenu(ThermoBlock::PIDHeater &thermoblock, WaterControl::Pump &pump);

extern void drawPumpControlMenu(WaterControl::Pump &pump);
extern void drawGrinderControlMenu(GrinderState *grinderStatus, double *setQuantity, double *currentQuantity);
extern void drawErrorScreen(ErrorCode code);

extern void updateWarmUpScreen(double percentage);
extern uint16_t updateMainMenu();
extern uint16_t updateCoffeeMenu(double *volume, double *temperature, double *quantity);
extern uint16_t updateMakeCoffeeScreen(double setTemp, double setVol, double setQty, double curTemp, double curVol, double curQty);
extern uint16_t updateControlMenu();
extern uint16_t updateThermoControlMenu();
extern uint16_t updateStepResponseMenu(ThermoBlock::PIDHeater &thermoblock, WaterControl::Pump &pump);
extern uint16_t updatePumpControlMenu(WaterControl::Pump &pump);
extern uint16_t updateGrinderControlMenu(GrinderState *grinderStatus, double *currentQuantity, double *setQuantity);
}

#endif