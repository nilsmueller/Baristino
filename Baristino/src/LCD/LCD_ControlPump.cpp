#include "LCD.h"

// Pump Control
Adafruit_GFX_Button btn_pump_minus;
Adafruit_GFX_Button btn_pump_plus;
Adafruit_GFX_Button btn_pump_onoff;
Adafruit_GFX_Button btn_pump_reset;
Adafruit_GFX_Button btn_pump_auto;
Adafruit_GFX_Button btn_pump_back;

namespace LCD {


WaterControl::WCState last_pump_onoff_state = WaterControl::WCState::OFF;


void pumpControlUpdateSetVolume(double value) {
    tft.setTextSize(4);
    tft.setTextColor(myBLUE, myBLACK);
    tft.setCursor(LCD_WIDTH/2 + 80, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD);

    if (value <= config::VOLUME_MIN) {
        value = config::VOLUME_MIN;
    }
    else if (value >= config::VOLUME_MAX) {
        value = config::VOLUME_MAX;
    }

    tft.print((int)value); 
    if (value < 100) {
        tft.print(" ");
    }
}


void pumpControlUpdateCurrentVolume(double volume) {
    tft.setTextSize(4);
    tft.setTextColor(myBLUE, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + 65, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD);
    tft.print((int)volume); 
    if (volume < 100) {
        tft.print("   ");
    }
}



// PUMP
void drawPumpControlMenu(WaterControl::Pump &pump) {
    drawEmptyScreenDouble();

    char btnLabel[9][7] = {"-", "+", "Back", "Reset", "On", "Off", "Auto", "Manual", "On/Off"};
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 2*LCD_PAD);
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);
    tft.print("Water Pump");
    
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 7*LCD_PAD);
    tft.print("Control");


    btn_pump_back.initButtonUL(&tft, 2*LCD_BORDER + 2*LCD_PAD, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myBLUE, myWHITE, btnLabel[2], 3);
    btn_pump_back.drawButton(false);

    if (pump.isOn()) {
        btn_pump_onoff.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myGREEN, myWHITE, btnLabel[4], 3);
        btn_pump_onoff.drawButton(false);
    }
    else {
        btn_pump_onoff.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myRED, myWHITE, btnLabel[5], 3);
        btn_pump_onoff.drawButton(false);
    }

    // Main 
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD);
    tft.print("Volume [mL]"); 

    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 + LCD_PAD, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD);
    tft.print("Pump   [mL]");  


    tft.setTextSize(4);
    tft.setTextColor(myBLUE);
    tft.setCursor(LCD_MAIN_ORIGIN_X + 65, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD);
    tft.print((int)pump.getCurrentAmount()); 


    tft.setCursor(LCD_WIDTH/2 + 80, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD);
    tft.print((int)pump.getSetAmount()); 

    btn_pump_minus.initButtonUL(&tft, LCD_WIDTH/2 + 1.5*LCD_PAD + LCD_BORDER, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD - 20, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[0], 4);
    btn_pump_minus.drawButton(false);

    btn_pump_plus.initButtonUL(&tft, LCD_WIDTH - LCD_MAIN_ORIGIN_X - 60, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD - 20, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[1], 4);
    btn_pump_plus.drawButton(false);

    btn_pump_reset.initButtonUL(&tft, 65, LCD_MAIN_ORIGIN_Y + 25*LCD_PAD, 120, 60, myBLUE, myYELLOW, myBLUE, btnLabel[3], 3);
    btn_pump_reset.drawButton(false);

    btn_pump_auto.initButtonUL(&tft, LCD_WIDTH/2 + 55, LCD_MAIN_ORIGIN_Y + 25*LCD_PAD, 120, 60, myBLUE, myYELLOW, myBLUE, btnLabel[6], 3);
    btn_pump_auto.drawButton(false);


}


uint16_t updatePumpControlMenu(WaterControl::Pump &pump) {
    
    bool isTouched = getTouchCoord();
    uint16_t menuID = pumpControlMenuID;
    btn_pump_minus.press(isTouched && btn_pump_minus.contains(touch_pixel_x, touch_pixel_y));
    btn_pump_plus.press(isTouched && btn_pump_plus.contains(touch_pixel_x, touch_pixel_y));
    btn_pump_reset.press(isTouched && btn_pump_reset.contains(touch_pixel_x, touch_pixel_y));
    btn_pump_auto.press(isTouched && btn_pump_auto.contains(touch_pixel_x, touch_pixel_y));
    btn_pump_onoff.press(isTouched && btn_pump_onoff.contains(touch_pixel_x, touch_pixel_y));
    btn_pump_back.press(isTouched && btn_pump_back.contains(touch_pixel_x, touch_pixel_y));


    char btnLabel[2][4] = {"On", "Off"};
    // toggle OnOff Button
    
    if (pump.isOn()) {
        btn_pump_onoff.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myGREEN, myWHITE, btnLabel[0], 3);
        btn_pump_onoff.drawButton(false);
    }
    else {
        btn_pump_onoff.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myRED, myWHITE, btnLabel[1], 3);
        btn_pump_onoff.drawButton(false);
    }


    if (btn_pump_minus.justReleased()) {
        btn_pump_minus.drawButton(false);
    }
    if (btn_pump_minus.justPressed()) {
        btn_pump_minus.drawButton(true);
        pump.setAmount(pump.getSetAmount() - 10.0);
        pumpControlUpdateSetVolume(pump.getSetAmount());
    }

    if (btn_pump_plus.justReleased()) {
        btn_pump_plus.drawButton(false);
    }
    if (btn_pump_plus.justPressed()) {
        btn_pump_plus.drawButton(true);
        pump.setAmount(pump.getSetAmount() + 10.0);
        pumpControlUpdateSetVolume(pump.getSetAmount());
    }

    if (btn_pump_reset.justReleased()) {
        btn_pump_reset.drawButton(false);
        pump.tareScale();
    }
    if (btn_pump_reset.justPressed()) {
        btn_pump_reset.drawButton(true);
    }

    if (btn_pump_onoff.justReleased()) {
        btn_pump_onoff.drawButton(false);
    }
    if (btn_pump_onoff.justPressed()) {
        btn_pump_onoff.drawButton(true);
        if (pump.isOff()) {
            pump.switchOn();
        }
        else if (pump.isOn())
        {
            pump.switchOff();
        }
        
    }

    if (btn_pump_auto.justReleased()) {
        btn_pump_auto.drawButton(false);
    }
    if (btn_pump_auto.justPressed()) {
        btn_pump_auto.drawButton(true);
        //*pumpStatus = PumpState::AUTO;
    }

    if (btn_pump_back.justReleased()) {
        btn_pump_back.drawButton(false);
    }
    if (btn_pump_back.justPressed()) {
        btn_pump_back.drawButton(true);
        //*pumpStatus = PumpState::OFF;
        menuID = controlMenuID;
    }


    pumpControlUpdateCurrentVolume(pump.getCurrentAmount());
    return menuID;
}


}