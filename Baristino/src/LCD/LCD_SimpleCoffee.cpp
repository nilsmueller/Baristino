#include "LCD.h"
#include "../configuration.h"

Adafruit_GFX_Button btn_smallSize;
Adafruit_GFX_Button btn_mediumSize;
Adafruit_GFX_Button btn_largeSize;
Adafruit_GFX_Button btn_weakDose;
Adafruit_GFX_Button btn_normalDose;
Adafruit_GFX_Button btn_strongDose;
Adafruit_GFX_Button btn_back;
Adafruit_GFX_Button btn_go;

enum Volume : uint8_t {
    SMALL,
    MEDIUM,
    LARGE
};

enum Dose : uint8_t {
    WEAK,
    NORMAL,
    STRONG
};

Volume selectedVolume = Volume::SMALL;
Dose selectedDose = Dose::WEAK;

void switchVolume() {
    switch (selectedVolume)
    {
    case Volume::SMALL:
        btn_smallSize.drawButton(true);
        btn_mediumSize.drawButton(false);
        btn_largeSize.drawButton(false);
        break;    
        
    case Volume::MEDIUM:
        btn_smallSize.drawButton(false);
        btn_mediumSize.drawButton(true);
        btn_largeSize.drawButton(false);
        break;
    
    case Volume::LARGE:
        btn_smallSize.drawButton(false);
        btn_mediumSize.drawButton(false);
        btn_largeSize.drawButton(true);
        break;
    }
}

void switchDose() {    
    switch (selectedDose)
    {
    case Dose::WEAK:
        btn_weakDose.drawButton(true);
        btn_normalDose.drawButton(false);
        btn_strongDose.drawButton(false);
        break;    
        
    case Dose::NORMAL:
        btn_weakDose.drawButton(false);
        btn_normalDose.drawButton(true);
        btn_strongDose.drawButton(false);
        break;
    
    case Dose::STRONG:
        btn_weakDose.drawButton(false);
        btn_normalDose.drawButton(false);
        btn_strongDose.drawButton(true);
        break;
    }
}

namespace LCD {

uint8_t simpleMenuID = 1;

void drawSimpleCoffeeMenu() {
    drawEmptyScreen();
    
    // Topbar
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 2*LCD_PAD);
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);
    tft.print("Select");
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 7*LCD_PAD);
    tft.print("Beverage");

    int w = 0.9*LCD_MAIN_WIDTH/3 - 1.5*LCD_PAD;
    int h = 0.9*LCD_MAIN_HEIGHT/2 - 1.5*LCD_PAD;


    char btnLabel[8][10] = {"small", "medium", "large", "weak", "normal", "strong", "back", "Go!"};
    btn_smallSize.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + 10, LCD_MAIN_ORIGIN_Y + LCD_PAD + 10, w, h, myBLUE, myYELLOW, myWHITE, btnLabel[0], 3);
    btn_smallSize.drawButton(false);
    btn_mediumSize.initButtonUL(&tft, LCD_MAIN_WIDTH / 2 - w/2, LCD_MAIN_ORIGIN_Y + LCD_PAD + 10, w, h, myBLUE, myYELLOW, myWHITE, btnLabel[1], 3);
    btn_mediumSize.drawButton(false);
    btn_largeSize.initButtonUL(&tft, LCD_MAIN_WIDTH - w - LCD_PAD, LCD_MAIN_ORIGIN_Y + LCD_PAD + 10, w, h, myBLUE, myYELLOW, myWHITE, btnLabel[2], 3);
    btn_largeSize.drawButton(false);

    btn_weakDose.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + 10, LCD_MAIN_ORIGIN_Y + 5*LCD_PAD + h, w, h, myBLUE, myYELLOW, myWHITE, btnLabel[3], 3);
    btn_weakDose.drawButton(false);
    btn_normalDose.initButtonUL(&tft, LCD_MAIN_WIDTH / 2 - w/2, LCD_MAIN_ORIGIN_Y + 5*LCD_PAD + h, w, h, myBLUE, myYELLOW, myWHITE, btnLabel[4], 3);
    btn_normalDose.drawButton(false);
    btn_strongDose.initButtonUL(&tft, LCD_MAIN_WIDTH - w - LCD_PAD, LCD_MAIN_ORIGIN_Y + 5*LCD_PAD + h, w, h, myBLUE, myYELLOW, myWHITE, btnLabel[5], 3);
    btn_strongDose.drawButton(false);

    btn_back.initButtonUL(&tft, 2*LCD_BORDER + 2*LCD_PAD, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myBLUE, myWHITE, btnLabel[6], 4);
    btn_back.drawButton(false);

    btn_go.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myRED, myWHITE, btnLabel[7], 4);
    btn_go.drawButton(false);
}


uint16_t updateSimpleCoffeeMenu(BrewParam *BrewParameter) {
    bool isTouched = getTouchCoord();
    uint16_t menuID = simpleMenuID;

    btn_smallSize.press(isTouched && btn_smallSize.contains(touch_pixel_x, touch_pixel_y));
    btn_mediumSize.press(isTouched && btn_mediumSize.contains(touch_pixel_x, touch_pixel_y));
    btn_largeSize.press(isTouched && btn_largeSize.contains(touch_pixel_x, touch_pixel_y));
    btn_weakDose.press(isTouched && btn_weakDose.contains(touch_pixel_x, touch_pixel_y));
    btn_normalDose.press(isTouched && btn_normalDose.contains(touch_pixel_x, touch_pixel_y));
    btn_strongDose.press(isTouched && btn_strongDose.contains(touch_pixel_x, touch_pixel_y));
    btn_back.press(isTouched && btn_back.contains(touch_pixel_x, touch_pixel_y));
    btn_go.press(isTouched && btn_go.contains(touch_pixel_x, touch_pixel_y));

    // VOLUME
    if (btn_smallSize.justPressed()) {
        btn_smallSize.drawButton(true);
        BrewParameter->set_volume = config::COFFEE_SMALL;
        selectedVolume = Volume::SMALL;
        switchVolume();
    }

    if (btn_mediumSize.justPressed()) {
        btn_mediumSize.drawButton(true);
        BrewParameter->set_volume = config::COFFEE_MEDIUM;
        selectedVolume = Volume::MEDIUM;
        switchVolume();
    }

    if (btn_largeSize.justPressed()) {
        btn_largeSize.drawButton(true);
        BrewParameter->set_volume = config::COFFEE_LARGE;
        selectedVolume = Volume::LARGE;
        switchVolume();
    }

    // DOSE
    if (btn_weakDose.justPressed()) {
        btn_weakDose.drawButton(true);
        BrewParameter->set_dose = BrewParameter->set_volume * config::COFFEE_WEAK_RATIO;
        selectedDose = Dose::WEAK;
        switchDose();
    }

    if (btn_normalDose.justPressed()) {
        btn_normalDose.drawButton(true);
        BrewParameter->set_dose = BrewParameter->set_volume * config::COFFEE_NORMAL_RATIO;
        selectedDose = Dose::NORMAL;
        switchDose();
    }

    if (btn_strongDose.justPressed()) {
        btn_strongDose.drawButton(true);
        BrewParameter->set_dose = BrewParameter->set_volume * config::COFFEE_STRONG_RATIO;
        selectedDose = Dose::STRONG;
        switchDose();
    }

    // MENU
    if (btn_go.justPressed()) {
        btn_go.drawButton(true);
        menuID = 21;
    }

    if (btn_back.justPressed()) {
        btn_back.drawButton(true);
        menuID = 0;
    }

    return menuID;
}

}