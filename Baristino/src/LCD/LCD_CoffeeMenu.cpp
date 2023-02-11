#include "LCD.h"
#include "../configuration.h"

namespace LCD {


Adafruit_GFX_Button btn_coffee_vol_plus;
Adafruit_GFX_Button btn_coffee_vol_minus;
Adafruit_GFX_Button btn_coffee_tmp_plus;
Adafruit_GFX_Button btn_coffee_tmp_minus;
Adafruit_GFX_Button btn_coffee_qty_plus;
Adafruit_GFX_Button btn_coffee_qty_minus;
Adafruit_GFX_Button btn_coffee_back;
Adafruit_GFX_Button btn_coffee_go;

uint16_t coffeMenuID = 1;


void settingsMenuUpdateVolume(double value, double *volume) {

  tft.setTextSize(4);
  tft.setTextColor(myBLUE, myBLACK);
  tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 + 60 + 2*LCD_PAD, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD + 15);

  if (value <= config::VOLUME_MIN) {
    value = config::VOLUME_MIN;
  }
  else if (value >= config::VOLUME_MAX) {
    value = config::VOLUME_MAX;
  }
  
  *volume = value;
  tft.print((int)*volume); 
  if (*volume < 100) {
    tft.print(" ");
  }
}


void settingsMenuUpdateTemperature(double value, double *temperature) {
    tft.setTextSize(4);
    tft.setTextColor(myBLUE, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 + 60 + 2*LCD_PAD, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD + 15);

    if (value <= config::TEMPERATURE_MIN) {
        value = config::TEMPERATURE_MIN;
    }
    else if (value >= config::TEMPERATURE_MAX) {
        value = config::TEMPERATURE_MAX;
    }
  
    *temperature = value;
    tft.print((int)*temperature);
    if (*temperature < 100) {
        tft.print(" ");
    }
}


void settingsMenuUpdateQuantity(double value, double *quantity) {

  tft.setTextSize(4);
  tft.setTextColor(myBLUE, myBLACK);
  tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 + 60 + 2*LCD_PAD, LCD_MAIN_ORIGIN_Y + 25*LCD_PAD + 15);

  if (value <= config::QUANTITY_MIN) {
    value = config::QUANTITY_MIN;
  }
  else if (value >= config::QUANTITY_MAX) {
    value = config::QUANTITY_MAX;
  }
  
  *quantity = value;
  tft.print((int)*quantity); 
  tft.print(" ");
}


void drawCoffeeMenu() {
    drawEmptyScreen();

    // Topbar
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 2*LCD_PAD);
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);
    tft.print("Set Brew");
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 7*LCD_PAD);
    tft.print("Parameters");

    // Main 
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD + 20);
    tft.print("Volume [mL]");  

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD + 20);
    tft.print("Temp.   [C]");  

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 25*LCD_PAD + 20);
    tft.print("Coffee  [g]");


    char btnLabel[4][6] = {"-", "+", "Back", "Go!"};
    btn_coffee_vol_minus.initButtonUL(&tft, LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - LCD_PAD, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[0], 6);
    btn_coffee_vol_minus.drawButton(false);

    btn_coffee_tmp_minus.initButtonUL(&tft, LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - LCD_PAD, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[0], 6);
    btn_coffee_tmp_minus.drawButton(false);

    btn_coffee_qty_minus.initButtonUL(&tft, LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - LCD_PAD, LCD_MAIN_ORIGIN_Y + 25*LCD_PAD, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[0], 6);
    btn_coffee_qty_minus.drawButton(false);

    btn_coffee_vol_plus.initButtonUL(&tft, LCD_WIDTH - LCD_MAIN_ORIGIN_X - 60, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[1], 6);
    btn_coffee_vol_plus.drawButton(false);

    btn_coffee_tmp_plus.initButtonUL(&tft, LCD_WIDTH - LCD_MAIN_ORIGIN_X - 60, LCD_MAIN_ORIGIN_Y + 13.5*LCD_PAD, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[1], 6);
    btn_coffee_tmp_plus.drawButton(false);

    btn_coffee_qty_plus.initButtonUL(&tft, LCD_WIDTH - LCD_MAIN_ORIGIN_X - 60, LCD_MAIN_ORIGIN_Y + 25*LCD_PAD, 60, 60, myBLUE, myYELLOW, myBLUE, btnLabel[1], 6);
    btn_coffee_qty_plus.drawButton(false);

    btn_coffee_back.initButtonUL(&tft, 2*LCD_BORDER + 2*LCD_PAD, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myBLUE, myWHITE, btnLabel[2], 4);
    btn_coffee_back.drawButton(false);

    btn_coffee_go.initButtonUL(&tft, LCD_WIDTH - 2*LCD_BORDER - 2*LCD_PAD - 120, 2*LCD_PAD + 2*LCD_BORDER, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myRED, myWHITE, btnLabel[3], 4);
    btn_coffee_go.drawButton(false);
}


uint16_t updateCoffeeMenu(BrewParam *brewParameter) {
//double *volume, double *temperature, double *quantity) {
    bool isTouched = getTouchCoord();

    uint16_t menuID = coffeMenuID;
    btn_coffee_vol_minus.press(isTouched && btn_coffee_vol_minus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_vol_plus.press(isTouched && btn_coffee_vol_plus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_tmp_minus.press(isTouched && btn_coffee_tmp_minus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_tmp_plus.press(isTouched && btn_coffee_tmp_plus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_qty_minus.press(isTouched && btn_coffee_qty_minus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_qty_plus.press(isTouched && btn_coffee_qty_plus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_go.press(isTouched && btn_coffee_go.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_back.press(isTouched && btn_coffee_back.contains(touch_pixel_x, touch_pixel_y));

    if (btn_coffee_vol_minus.justReleased()) {
        btn_coffee_vol_minus.drawButton(false);
    }
    if (btn_coffee_vol_minus.justPressed()) {
        btn_coffee_vol_minus.drawButton(true);
        settingsMenuUpdateVolume(brewParameter->set_volume - 10.0, &brewParameter->set_volume);
    }


    if (btn_coffee_vol_plus.justReleased()) {
        btn_coffee_vol_plus.drawButton(false);
    }
    if (btn_coffee_vol_plus.justPressed()) {
        btn_coffee_vol_plus.drawButton(true);
        settingsMenuUpdateVolume(brewParameter->set_volume + 10.0, &brewParameter->set_volume);
    }    


    if (btn_coffee_tmp_minus.justReleased()) {
        btn_coffee_tmp_minus.drawButton(false);
    }
    if (btn_coffee_tmp_minus.justPressed()) {
        btn_coffee_tmp_minus.drawButton(true);
        settingsMenuUpdateVolume(brewParameter->set_temperature - 10.0, &brewParameter->set_temperature);
    }


    if (btn_coffee_tmp_plus.justReleased()) {
        btn_coffee_tmp_plus.drawButton(false);
    }
    if (btn_coffee_tmp_plus.justPressed()) {
        btn_coffee_tmp_plus.drawButton(true);
        settingsMenuUpdateTemperature(brewParameter->set_temperature + 10.0, &brewParameter->set_temperature);
    }    


    if (btn_coffee_qty_minus.justReleased()) {
        btn_coffee_qty_minus.drawButton(false);
    }
    if (btn_coffee_qty_minus.justPressed()) {
        settingsMenuUpdateQuantity(brewParameter->set_dose - 0.1, &brewParameter->set_dose);
        btn_coffee_qty_minus.drawButton(true);
    }


    if (btn_coffee_qty_plus.justReleased()) {
        btn_coffee_qty_plus.drawButton(false);
    }
    if (btn_coffee_qty_plus.justPressed()) {
        btn_coffee_qty_plus.drawButton(true);
        settingsMenuUpdateQuantity(brewParameter->set_dose + 0.1, &brewParameter->set_dose);
    }


    if (btn_coffee_go.justReleased()) {
        btn_coffee_go.drawButton(false);
    }
    if (btn_coffee_go.justPressed()) {
        btn_coffee_go.drawButton(true);
        menuID = 11;
    }



    if (btn_coffee_back.justReleased()) {
        btn_coffee_back.drawButton(false);
    }
    if (btn_coffee_back.justPressed()) {
        btn_coffee_back.drawButton(true);
        menuID = 0;
    }

    return menuID;

}


}