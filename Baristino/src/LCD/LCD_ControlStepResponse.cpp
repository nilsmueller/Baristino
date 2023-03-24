#include "LCD.h"

Adafruit_GFX_Button btn_power_minus;
Adafruit_GFX_Button btn_power_plus;
Adafruit_GFX_Button btn_stepresponse_startstop;
Adafruit_GFX_Button btn_stepresponse_pump_onoff;
Adafruit_GFX_Button btn_stepresponse_back;


namespace LCD {

// PUMP
void drawStepResponseMenu(ThermoBlock::PIDHeater &thermoblock, WaterControl::Pump &pump) {
drawEmptyScreen();

    // Topbar
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 2*LCD_PAD);
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);
    tft.print("Step Response");
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 7*LCD_PAD);
    tft.print("Menu");

    // Main 
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD + 20);
    tft.print("Power   [%]");  

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD + 20);
    tft.print("water flow ");  


    char btnLabel[6][6] = {"-", "+", "On", "Off", "Start", "Stop"};
    btn_power_minus.initButtonUL(&tft, LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - LCD_PAD, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[0], 6);
    btn_power_minus.drawButton(false);

    btn_power_plus.initButtonUL(&tft, LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - LCD_PAD, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[0], 6);
    btn_power_plus.drawButton(false);

    btn_stepresponse_pump_onoff.initButtonUL(&tft, LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - LCD_PAD, LCD_MAIN_ORIGIN_Y + 25*LCD_PAD, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[3], 6);
    btn_stepresponse_startstop.drawButton(false);

    tft.setTextSize(4);
    tft.setTextColor(myBLUE);
    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 + 60 + 2*LCD_PAD, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD + 15);
    tft.print((int)(thermoblock.getDutyCycle() * 100)); 

    btn_stepresponse_startstop.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myRED, myWHITE, btnLabel[3], 4);
    btn_stepresponse_startstop.drawButton(false);
}


uint16_t updateStepResponseMenu(ThermoBlock::PIDHeater &thermoblock, WaterControl::Pump &pump) {
    bool isTouched = getTouchCoord();
    uint16_t menuID = stepResponseMenuID;

    btn_power_minus.press(isTouched && btn_power_minus.contains(touch_pixel_x, touch_pixel_y));
    btn_power_plus.press(isTouched && btn_power_plus.contains(touch_pixel_x, touch_pixel_y));
    btn_stepresponse_startstop.press(isTouched && btn_stepresponse_startstop.contains(touch_pixel_x, touch_pixel_y));
    btn_stepresponse_pump_onoff.press(isTouched && btn_stepresponse_pump_onoff.contains(touch_pixel_x, touch_pixel_y));
    btn_stepresponse_back.press(isTouched && btn_stepresponse_back.contains(touch_pixel_x, touch_pixel_y));


    char btnLabel[6][6] = {"-", "+", "On", "Off", "Start", "Stop"};

    // START/STOP BUTTON
    if (thermoblock.isHeating()) {
        btn_stepresponse_startstop.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myGREEN, myWHITE, btnLabel[5], 3);
        btn_stepresponse_startstop.drawButton(false);
    }
    else {
        btn_stepresponse_startstop.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myRED, myWHITE, btnLabel[4], 3);
        btn_stepresponse_startstop.drawButton(false);
    }



    if (btn_power_minus.justReleased()) {
        btn_power_minus.drawButton(false);
    }
    if (btn_power_minus.justPressed()) {
        btn_power_minus.drawButton(true);
    }

    if (btn_power_plus.justReleased()) {
        btn_power_plus.drawButton(false);
    }
    if (btn_power_plus.justPressed()) {
        btn_power_plus.drawButton(true);
    }



    return menuID;
}




}