#include "LCD.h"


namespace LCD {


// Grinder Control
Adafruit_GFX_Button btn_grinder_minus;
Adafruit_GFX_Button btn_grinder_plus;
Adafruit_GFX_Button btn_grinder_onoff;
Adafruit_GFX_Button btn_grinder_reset;
Adafruit_GFX_Button btn_grinder_auto;
Adafruit_GFX_Button btn_grinder_back;


uint16_t grinderControlMenuID = 33;

GrinderState last_grinder_onoff_state = GrinderState::OFF;


void grinderControlUpdateSetQuantity(double value, double *quantity) {
    tft.setTextSize(4);
    tft.setTextColor(myBLUE, myBLACK);
    tft.setCursor(LCD_WIDTH/2 + 80, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD);

    if (value <= config::QUANTITY_MIN) {
        value = config::QUANTITY_MIN;
    }
    else if (value >= config::QUANTITY_MAX) {
        value = config::QUANTITY_MAX;
    }

    *quantity = value;
    tft.print((int)*quantity); 
    if (*quantity < 100) {
        tft.print(" ");
    }
}


void grinderControlUpdateCurrentQuantity(double *quantity) {
    tft.setTextSize(4);
    tft.setTextColor(myBLUE, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + 65, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD);
    tft.print((int)*quantity); 
    if (*quantity < 100) {
        tft.print("   ");
    }
}



// PUMP
void drawGrinderControlMenu(GrinderState *grinderStatus, double *setQuantity, double *currentQuantity) {
    //drawEmptyScreen();
    drawEmptyScreenDouble();

    char btnLabel[9][7] = {"-", "+", "Back", "Reset", "On", "Off", "Auto", "Manual", "On/Off"};
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 2*LCD_PAD);
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);
    tft.print("Grinder");
    
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 7*LCD_PAD);
    tft.print("Control");


    btn_grinder_back.initButtonUL(&tft, 2*LCD_BORDER + 2*LCD_PAD, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myBLUE, myWHITE, btnLabel[2], 3);
    btn_grinder_back.drawButton(false);

    if (*grinderStatus == GrinderState::ON || *grinderStatus == GrinderState::AUTO) {
            btn_grinder_onoff.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myGREEN, myWHITE, btnLabel[4], 3);
            btn_grinder_onoff.drawButton(false);
        }
    else {
        btn_grinder_onoff.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myRED, myWHITE, btnLabel[5], 3);
        btn_grinder_onoff.drawButton(false);
    }

    // Main 
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD);
    tft.print("Coffee  [g]"); 

    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 + LCD_PAD, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD);
    tft.print("Grind   [g]");  


    tft.setTextSize(4);
    tft.setTextColor(myBLUE);
    tft.setCursor(LCD_MAIN_ORIGIN_X + 65, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD);
    tft.print((int)*currentQuantity); 


    tft.setCursor(LCD_WIDTH/2 + 80, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD);
    tft.print((int)*setQuantity); 

    btn_grinder_minus.initButtonUL(&tft, LCD_WIDTH/2 + 1.5*LCD_PAD + LCD_BORDER, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD - 20, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[0], 4);
    btn_grinder_minus.drawButton(false);

    btn_grinder_plus.initButtonUL(&tft, LCD_WIDTH - LCD_MAIN_ORIGIN_X - 60, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD - 20, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[1], 4);
    btn_grinder_plus.drawButton(false);

    btn_grinder_reset.initButtonUL(&tft, 65, LCD_MAIN_ORIGIN_Y + 25*LCD_PAD, 120, 60, myBLUE, myYELLOW, myBLUE, btnLabel[3], 3);
    btn_grinder_reset.drawButton(false);

    btn_grinder_auto.initButtonUL(&tft, LCD_WIDTH/2 + 55, LCD_MAIN_ORIGIN_Y + 25*LCD_PAD, 120, 60, myBLUE, myYELLOW, myBLUE, btnLabel[6], 3);
    btn_grinder_auto.drawButton(false);


}


uint16_t updateGrinderControlMenu(GrinderState *grinderStatus, double *setQuantity, double *currentQuantity) {
    
    bool isTouched = getTouchCoord();
    uint8_t menuID = grinderControlMenuID;
    btn_grinder_minus.press(isTouched && btn_grinder_minus.contains(touch_pixel_x, touch_pixel_y));
    btn_grinder_plus.press(isTouched && btn_grinder_plus.contains(touch_pixel_x, touch_pixel_y));
    btn_grinder_reset.press(isTouched && btn_grinder_reset.contains(touch_pixel_x, touch_pixel_y));
    btn_grinder_auto.press(isTouched && btn_grinder_auto.contains(touch_pixel_x, touch_pixel_y));
    btn_grinder_onoff.press(isTouched && btn_grinder_onoff.contains(touch_pixel_x, touch_pixel_y));
    btn_grinder_back.press(isTouched && btn_grinder_back.contains(touch_pixel_x, touch_pixel_y));


    char btnLabel[2][4] = {"On", "Off"};
    // toggle OnOff Button
    if (last_grinder_onoff_state != *grinderStatus) {
        if (*grinderStatus == GrinderState::ON || *grinderStatus == GrinderState::AUTO) {
            btn_grinder_onoff.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myGREEN, myWHITE, btnLabel[0], 3);
            btn_grinder_onoff.drawButton(false);
        }
        else {
            btn_grinder_onoff.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myRED, myWHITE, btnLabel[1], 3);
            btn_grinder_onoff.drawButton(false);
        }
        last_grinder_onoff_state = *grinderStatus;
    }

    if (btn_grinder_minus.justReleased()) {
        btn_grinder_minus.drawButton(false);
    }
    if (btn_grinder_minus.justPressed()) {
        btn_grinder_minus.drawButton(true);
        grinderControlUpdateSetQuantity(*setQuantity - 0.1, setQuantity);
    }

    if (btn_grinder_plus.justReleased()) {
        btn_grinder_plus.drawButton(false);
    }
    if (btn_grinder_plus.justPressed()) {
        btn_grinder_plus.drawButton(true);
        grinderControlUpdateSetQuantity(*setQuantity + 0.1, setQuantity);
    }

    if (btn_grinder_reset.justReleased()) {
        btn_grinder_reset.drawButton(false);
        *currentQuantity = 0.0;
    }
    if (btn_grinder_reset.justPressed()) {
        btn_grinder_reset.drawButton(true);
    }

    if (btn_grinder_onoff.justReleased()) {
        btn_grinder_onoff.drawButton(false);
    }
    if (btn_grinder_onoff.justPressed()) {
        btn_grinder_onoff.drawButton(true);
        if (*grinderStatus == GrinderState::OFF) {
            *grinderStatus = GrinderState::ON;
        }
        else if (*grinderStatus == GrinderState::ON)
        {
            *grinderStatus = GrinderState::OFF;
        }
        
    }

    if (btn_grinder_auto.justReleased()) {
        btn_grinder_auto.drawButton(false);
    }
    if (btn_grinder_auto.justPressed()) {
        btn_grinder_auto.drawButton(true);
        *grinderStatus = GrinderState::AUTO;
    }

    if (btn_grinder_back.justReleased()) {
        btn_grinder_back.drawButton(false);
    }
    if (btn_grinder_back.justPressed()) {
        btn_grinder_back.drawButton(true);
        //*grinderStatus = GrinderState::OFF;
        menuID = LCD::controlMenuID;
    }


    grinderControlUpdateCurrentQuantity(currentQuantity);
    return menuID;
}


}