#include "LCD.h"


Adafruit_GFX_Button btn_thermoblock_pid;
Adafruit_GFX_Button btn_thermoblock_stepresponse;
Adafruit_GFX_Button btn_thermoblock_back;

namespace LCD {

// PUMP
void drawThermoControlMenu() {
    //drawEmptyScreen();
    drawEmptyScreenDouble();

    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 2*LCD_PAD);
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);
    tft.print("ThermoBlock");
    
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 7*LCD_PAD);
    tft.print("Control");


    int width_button_main = 0.9*LCD_MAIN_WIDTH/2 - 1.5*LCD_PAD;
    int height_button_main = 0.9*LCD_MAIN_HEIGHT/2 - 1.5*LCD_PAD;

    // Main
    int mainpad_x = (LCD_MAIN_WIDTH - 2 * width_button_main) / 3;
    int mainpad_y = (LCD_MAIN_HEIGHT - 2 * height_button_main) / 3;

    char btnLabel[3][10] = {"Back", "PID", "Step"};
    btn_thermoblock_pid.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + mainpad_x, LCD_MAIN_ORIGIN_Y + mainpad_y, width_button_main, height_button_main, myBLUE, myYELLOW, myWHITE, btnLabel[0], 3);
    btn_thermoblock_pid.drawButton(false);

    btn_thermoblock_stepresponse.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + width_button_main + 2* mainpad_x, LCD_MAIN_ORIGIN_Y + mainpad_y, width_button_main, height_button_main, myBLUE, myYELLOW, myWHITE, btnLabel[1], 3);
    btn_thermoblock_stepresponse.drawButton(false);

    btn_thermoblock_back.initButtonUL(&tft, 2*LCD_BORDER + 2*LCD_PAD, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myBLUE, myWHITE, btnLabel[4], 4);
    btn_thermoblock_back.drawButton(false);
}


uint16_t updateThermoControlMenu() {
    
    bool isTouched = getTouchCoord();
    uint16_t menuID = thermoblockControlMenuID;

    btn_thermoblock_pid.press(isTouched && btn_thermoblock_pid.contains(touch_pixel_x, touch_pixel_y));
    btn_thermoblock_stepresponse.press(isTouched && btn_thermoblock_stepresponse.contains(touch_pixel_x, touch_pixel_y));
    btn_thermoblock_back.press(isTouched && btn_thermoblock_back.contains(touch_pixel_x, touch_pixel_y));

    // switch to ThermoBlock PID SubMenu
    if (btn_thermoblock_pid.justReleased()) {
        btn_thermoblock_pid.drawButton(false);
    }
    if (btn_thermoblock_pid.justPressed()) {
        //menuID = 3;//31;
        btn_thermoblock_pid.drawButton(true);
    }


    // switch to ThermoBlock StepResponse SubMenu
    if (btn_thermoblock_stepresponse.justReleased()) {
        btn_thermoblock_stepresponse.drawButton(false);
    }
    if (btn_thermoblock_stepresponse.justPressed()) {
        menuID = stepResponseMenuID;
        btn_thermoblock_stepresponse.drawButton(true);
    }


    // switch back to Control bMenu
    if (btn_thermoblock_back.justReleased()) {
        btn_thermoblock_back.drawButton(false);
    }
    if (btn_thermoblock_back.justPressed()) {
        menuID = controlMenuID;
        btn_thermoblock_back.drawButton(true);
    }

    return menuID;
}


}