#include "LCD.h"
#include "../configuration.h"


Adafruit_GFX_Button btn_coffee_vol_plus;
Adafruit_GFX_Button btn_coffee_vol_minus;
Adafruit_GFX_Button btn_coffee_tmp_plus;
Adafruit_GFX_Button btn_coffee_tmp_minus;
Adafruit_GFX_Button btn_coffee_qty_plus;
Adafruit_GFX_Button btn_coffee_qty_minus;
Adafruit_GFX_Button btn_coffee_pre_plus;
Adafruit_GFX_Button btn_coffee_pre_minus;
Adafruit_GFX_Button btn_coffee_blm_plus;
Adafruit_GFX_Button btn_coffee_blm_minus;
Adafruit_GFX_Button btn_coffee_back;
Adafruit_GFX_Button btn_coffee_go;


namespace LCD {

void settingsMenuUpdateVolume(double value, double *volume) {

  tft.setTextSize(4);
  tft.setTextColor(myBLUE, myBLACK);
  tft.setCursor(85, 96);

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
    tft.setCursor(85, 172);

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
  tft.setCursor(85, 250);

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


void settingsMenuUpdatePreinfusion(double value, double *preinf) {
    tft.setTextSize(4);
    tft.setTextColor(myBLUE, myBLACK);
    tft.setCursor(LCD_WIDTH/2 + 85, 96);

    if (value <= config::PREINFUSION_MIN) {
        value = config::PREINFUSION_MIN;
    }
    else if (value >= config::PREINFUSION_MAX) {
        value = config::PREINFUSION_MAX;
    }
  
    *preinf= value;
    tft.print((int)*preinf);
    if (*preinf < 100) {
        tft.print(" ");
    }
}


void settingsMenuUpdateBloom(double value, double *bloom) {

  tft.setTextSize(4);
  tft.setTextColor(myBLUE, myBLACK);
  tft.setCursor(LCD_WIDTH/2 + 85, 172);

  if (value <= config::BLOOM_MIN) {
    value = config::BLOOM_MIN;
  }
  else if (value >= config::BLOOM_MAX) {
    value = config::BLOOM_MAX;
  }
  
  *bloom = value;
  tft.print((int)*bloom); 
  tft.print(" ");
}


void drawCustomMenu(BrewParam *brewParameter) {
    drawEmptyScreen();

    // Topbar
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 150, 10);
    tft.setTextColor(myWHITE);
    tft.setTextSize(2);
    tft.print("Adjust");
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 150, 30);
    tft.print("Parameters");

    // Main 
    tft.setTextColor(myWHITE, myBLACK);
    tft.setTextSize(2);

    tft.setCursor(LCD_MAIN_ORIGIN_X, 65);
    tft.print("Volume        [mL]");  

    tft.setCursor(LCD_MAIN_ORIGIN_X, 142);
    tft.print("Temp.          [C]");  

    tft.setCursor(LCD_MAIN_ORIGIN_X, 220);
    tft.print("Coffee         [g]");

    tft.setCursor(LCD_WIDTH/2 + 12, 65);
    tft.print("Preinfusion    [s]");  

    tft.setCursor(LCD_WIDTH/2 + 12, 142);
    tft.print("Bloom          [s]");

    tft.setTextSize(4);
    tft.setTextColor(myBLUE, myBLACK);
    tft.setCursor(85, 96);
    tft.print((int)brewParameter->set_volume);
    tft.setCursor(85, 172);
    tft.print((int)brewParameter->set_temperature);
    tft.setCursor(85, 250);
    tft.print((int)brewParameter->set_dose);    
    
    tft.setCursor(LCD_WIDTH/2 + 85, 96);
    tft.print((int) (brewParameter->set_preinfusion / 1000));
    tft.setCursor(LCD_WIDTH/2 + 85, 172);
    tft.print((int) (brewParameter->set_bloom / 1000));

    tft.drawFastVLine(LCD_WIDTH/2, 60, 232, myBLUE);

    char btnLabel[4][6] = {"-", "+", "Back", "Go!"};
    btn_coffee_vol_minus.initButtonUL(&tft, 10, 86, 70, 50, myBLUE, myYELLOW, myBLUE, btnLabel[0], 6);
    btn_coffee_vol_minus.drawButton(false);

    btn_coffee_tmp_minus.initButtonUL(&tft, 10, 162, 70, 50, myBLUE, myYELLOW, myBLUE, btnLabel[0], 6);
    btn_coffee_tmp_minus.drawButton(false);

    btn_coffee_qty_minus.initButtonUL(&tft, 10, 240, 70, 50, myBLUE, myYELLOW, myBLUE, btnLabel[0], 6);
    btn_coffee_qty_minus.drawButton(false);

    btn_coffee_vol_plus.initButtonUL(&tft, 160, 86, 70, 50, myBLUE, myYELLOW, myBLUE, btnLabel[1], 6);
    btn_coffee_vol_plus.drawButton(false);

    btn_coffee_tmp_plus.initButtonUL(&tft, 160, 162, 70, 50, myBLUE, myYELLOW, myBLUE, btnLabel[1], 6);
    btn_coffee_tmp_plus.drawButton(false);

    btn_coffee_qty_plus.initButtonUL(&tft, 160, 240, 70, 50, myBLUE, myYELLOW, myBLUE, btnLabel[1], 6);
    btn_coffee_qty_plus.drawButton(false);


    btn_coffee_pre_minus.initButtonUL(&tft, LCD_WIDTH/2 + 10, 86, 70, 50, myBLUE, myYELLOW, myBLUE, btnLabel[0], 6);
    btn_coffee_pre_minus.drawButton(false);

    btn_coffee_blm_minus.initButtonUL(&tft, LCD_WIDTH/2 + 10, 162, 70, 50, myBLUE, myYELLOW, myBLUE, btnLabel[0], 6);
    btn_coffee_blm_minus.drawButton(false);

    btn_coffee_pre_plus.initButtonUL(&tft, LCD_WIDTH/2 + 160, 86, 70, 50, myBLUE, myYELLOW, myBLUE, btnLabel[1], 6);
    btn_coffee_pre_plus.drawButton(false);

    btn_coffee_blm_plus.initButtonUL(&tft, LCD_WIDTH/2 + 160, 162, 70, 50, myBLUE, myYELLOW, myBLUE, btnLabel[1], 6);
    btn_coffee_blm_plus.drawButton(false);


    btn_coffee_back.initButtonUL(&tft, 10, 8, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myBLUE, myWHITE, btnLabel[2], 4);
    btn_coffee_back.drawButton(false);

    btn_coffee_go.initButtonUL(&tft, LCD_WIDTH - 130, 8, 120, LCD_TOPBAR_HEIGHT-2*LCD_PAD, myWHITE, myRED, myWHITE, btnLabel[3], 4);
    btn_coffee_go.drawButton(false);
}


uint16_t updateCustomMenu(BrewParam *brewParameter) {
//double *volume, double *temperature, double *quantity) {
    bool isTouched = getTouchCoord();

    uint16_t menuID = customCoffeeMenuID;
    btn_coffee_vol_minus.press(isTouched && btn_coffee_vol_minus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_vol_plus.press(isTouched && btn_coffee_vol_plus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_tmp_minus.press(isTouched && btn_coffee_tmp_minus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_tmp_plus.press(isTouched && btn_coffee_tmp_plus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_qty_minus.press(isTouched && btn_coffee_qty_minus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_qty_plus.press(isTouched && btn_coffee_qty_plus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_pre_minus.press(isTouched && btn_coffee_pre_minus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_pre_plus.press(isTouched && btn_coffee_pre_plus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_blm_minus.press(isTouched && btn_coffee_blm_minus.contains(touch_pixel_x, touch_pixel_y));
    btn_coffee_blm_plus.press(isTouched && btn_coffee_blm_plus.contains(touch_pixel_x, touch_pixel_y));
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
        settingsMenuUpdateTemperature(brewParameter->set_temperature - 1.0, &brewParameter->set_temperature);
    }


    if (btn_coffee_tmp_plus.justReleased()) {
        btn_coffee_tmp_plus.drawButton(false);
    }
    if (btn_coffee_tmp_plus.justPressed()) {
        btn_coffee_tmp_plus.drawButton(true);
        settingsMenuUpdateTemperature(brewParameter->set_temperature + 1.0, &brewParameter->set_temperature);
    }    


    if (btn_coffee_qty_minus.justReleased()) {
        btn_coffee_qty_minus.drawButton(false);
    }
    if (btn_coffee_qty_minus.justPressed()) {
        settingsMenuUpdateQuantity(brewParameter->set_dose - 1, &brewParameter->set_dose);
        btn_coffee_qty_minus.drawButton(true);
    }


    if (btn_coffee_qty_plus.justReleased()) {
        btn_coffee_qty_plus.drawButton(false);
    }
    if (btn_coffee_qty_plus.justPressed()) {
        btn_coffee_qty_plus.drawButton(true);
        settingsMenuUpdateQuantity(brewParameter->set_dose + 1, &brewParameter->set_dose);
    }


    if (btn_coffee_pre_minus.justReleased()) {
        btn_coffee_pre_minus.drawButton(false);
    }
    if (btn_coffee_pre_minus.justPressed()) {
        settingsMenuUpdatePreinfusion(brewParameter->set_preinfusion - 1, &brewParameter->set_preinfusion);
        btn_coffee_pre_minus.drawButton(true);
    }


    if (btn_coffee_pre_plus.justReleased()) {
        btn_coffee_pre_plus.drawButton(false);
    }
    if (btn_coffee_pre_plus.justPressed()) {
        btn_coffee_pre_plus.drawButton(true);
        settingsMenuUpdatePreinfusion(brewParameter->set_preinfusion + 1, &brewParameter->set_preinfusion);
    }

    
    if (btn_coffee_blm_minus.justReleased()) {
        btn_coffee_blm_minus.drawButton(false);
    }
    if (btn_coffee_blm_minus.justPressed()) {
        settingsMenuUpdateBloom(brewParameter->set_bloom - 1, &brewParameter->set_bloom);
        btn_coffee_blm_minus.drawButton(true);
    }


    if (btn_coffee_blm_plus.justReleased()) {
        btn_coffee_blm_plus.drawButton(false);
    }
    if (btn_coffee_blm_plus.justPressed()) {
        btn_coffee_blm_plus.drawButton(true);
        settingsMenuUpdateBloom(brewParameter->set_bloom + 1, &brewParameter->set_bloom);
    }


    if (btn_coffee_go.justReleased()) {
        btn_coffee_go.drawButton(false);
    }
    if (btn_coffee_go.justPressed()) {
        btn_coffee_go.drawButton(true);
        menuID = 21;
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