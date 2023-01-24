#include "LCD.h"
#include "../configuration.h"

namespace LCD {

Adafruit_GFX_Button btn_control_thermoblock;
Adafruit_GFX_Button btn_control_brewgroup;
Adafruit_GFX_Button btn_control_grinder;
Adafruit_GFX_Button btn_control_pump;
Adafruit_GFX_Button btn_control_back;

uint8_t controlMenuID = 3;


void drawControlMenu() {
    drawEmptyScreen();
    // Topbar
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 2*LCD_PAD);
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);
    tft.print("Peripherals");
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 7*LCD_PAD);
    tft.print("Control Menu");

    int width_button_main = 0.9*LCD_MAIN_WIDTH/2 - 1.5*LCD_PAD;
    int height_button_main = 0.9*LCD_MAIN_HEIGHT/2 - 1.5*LCD_PAD;

    // Main
    int mainpad_x = (LCD_MAIN_WIDTH - 2 * width_button_main) / 3;
    int mainpad_y = (LCD_MAIN_HEIGHT - 2 * height_button_main) / 3;

    char btnLabel[5][12] = {"ThermoBlock", "BrewGroup", "Grinder", "Pump", "Back"};
    btn_control_thermoblock.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + mainpad_x, LCD_MAIN_ORIGIN_Y + mainpad_y, width_button_main, height_button_main, myBLUE, myYELLOW, myWHITE, btnLabel[0], 3);
    btn_control_thermoblock.drawButton(false);

    btn_control_brewgroup.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + width_button_main + 2* mainpad_x, LCD_MAIN_ORIGIN_Y + mainpad_y, width_button_main, height_button_main, myBLUE, myYELLOW, myWHITE, btnLabel[1], 3);
    btn_control_brewgroup.drawButton(false);

    btn_control_grinder.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + mainpad_x, LCD_MAIN_ORIGIN_Y + 2*mainpad_y + height_button_main, width_button_main, height_button_main, myBLUE, myYELLOW, myWHITE, btnLabel[2], 3);
    btn_control_grinder.drawButton(false);

    btn_control_pump.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + width_button_main + 2* mainpad_x, LCD_MAIN_ORIGIN_Y + 2*mainpad_y + height_button_main, width_button_main, height_button_main, myBLUE, myYELLOW, myWHITE, btnLabel[3], 3);
    btn_control_pump.drawButton(false);

    btn_control_back.initButtonUL(&tft, 2*LCD_BORDER + 2*LCD_PAD, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myBLUE, myWHITE, btnLabel[4], 4);
    btn_control_back.drawButton(false);
}


// Returns the ID of the selected Menu. If none is selected the ID 0 is returned
// which is the ID of the Main Menu.
unsigned int updateControlMenu() {
    bool isTouched = getTouchCoord();
    uint8_t menuID = controlMenuID;
    btn_control_thermoblock.press(isTouched && btn_control_thermoblock.contains(touch_pixel_x, touch_pixel_y));
    btn_control_brewgroup.press(isTouched && btn_control_brewgroup.contains(touch_pixel_x, touch_pixel_y));
    btn_control_grinder.press(isTouched && btn_control_grinder.contains(touch_pixel_x, touch_pixel_y));
    btn_control_pump.press(isTouched && btn_control_pump.contains(touch_pixel_x, touch_pixel_y));
    btn_control_back.press(isTouched && btn_control_back.contains(touch_pixel_x, touch_pixel_y));

    // switch to ThermoBlock SubMenu
    if (btn_control_thermoblock.justReleased()) {
        btn_control_thermoblock.drawButton(false);
    }
    if (btn_control_thermoblock.justPressed()) {
        menuID = 3;//31;
        btn_control_thermoblock.drawButton(true);
    }

    // Switch to BrewGroup SubMenu
    if (btn_control_brewgroup.justReleased()) {
        btn_control_brewgroup.drawButton(false);
    }
    if (btn_control_brewgroup.justPressed()) {
        menuID = 3;//32;
        btn_control_brewgroup.drawButton(true);
    }

    // Switch to Grinder SubMenu
    if (btn_control_grinder.justReleased()) {
        btn_control_grinder.drawButton(false);
    }
    if (btn_control_grinder.justPressed()) {
        menuID = grinderControlMenuID;
        btn_control_grinder.drawButton(true);
    }

    // Switch to Pump SubMenu
    if (btn_control_pump.justReleased()) {
        btn_control_pump.drawButton(false);
    }
    if (btn_control_pump.justPressed()) {
        menuID = pumpControlMenuID;
        btn_control_pump.drawButton(true);
    }

    if (btn_control_back.justReleased()) {
        btn_control_back.drawButton(false);
    }
    if (btn_control_back.justPressed()) {
        menuID = 0;
        btn_control_back.drawButton(true);
    }

    return menuID;
}



}