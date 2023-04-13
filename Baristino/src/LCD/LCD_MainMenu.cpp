#include "LCD.h"

// Buttons Main Menu
Adafruit_GFX_Button btn_main_coffee;
Adafruit_GFX_Button btn_main_custom;
Adafruit_GFX_Button btn_main_control;
Adafruit_GFX_Button btn_main_settings;

namespace LCD {

void drawMainMenu() {
    drawEmptyScreen();

    // Topbar
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 2*LCD_PAD);
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);
    tft.print("Baristino");
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 7*LCD_PAD);
    tft.print("Main Menu");

    int width_button_main = 0.9*LCD_MAIN_WIDTH/2 - 1.5*LCD_PAD;
    int height_button_main = 0.9*LCD_MAIN_HEIGHT/2 - 1.5*LCD_PAD;

    // Main
    int mainpad_x = (LCD_MAIN_WIDTH - 2 * width_button_main) / 3;
    int mainpad_y = (LCD_MAIN_HEIGHT - 2 * height_button_main) / 3;

    char btnLabel[4][10] = {"Coffee", "Custom", "Control", "Settings"};
    btn_main_coffee.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + mainpad_x, LCD_MAIN_ORIGIN_Y + mainpad_y, width_button_main, height_button_main, myBLUE, myYELLOW, myWHITE, btnLabel[0], 3);
    btn_main_coffee.drawButton(false);

    btn_main_custom.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + width_button_main + 2* mainpad_x, LCD_MAIN_ORIGIN_Y + mainpad_y, width_button_main, height_button_main, myBLUE, myYELLOW, myWHITE, btnLabel[1], 3);
    btn_main_custom.drawButton(false);

    btn_main_control.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + mainpad_x, LCD_MAIN_ORIGIN_Y + 2*mainpad_y + height_button_main, width_button_main, height_button_main, myBLUE, myYELLOW, myWHITE, btnLabel[2], 3);
    btn_main_control.drawButton(false);

    btn_main_settings.initButtonUL(&tft, LCD_MAIN_ORIGIN_X - LCD_PAD + width_button_main + 2* mainpad_x, LCD_MAIN_ORIGIN_Y + 2*mainpad_y + height_button_main, width_button_main, height_button_main, myBLUE, myYELLOW, myWHITE, btnLabel[3], 3);
    btn_main_settings.drawButton(false);
} 


// Returns the ID of the selected Menu. If none is selected the ID 0 is returned
// which is the ID of the Main Menu.
uint16_t updateMainMenu() {
  bool isTouched = getTouchCoord();
  uint8_t menuID = mainMenuID;
  btn_main_coffee.press(isTouched && btn_main_coffee.contains(touch_pixel_x, touch_pixel_y));
  btn_main_custom.press(isTouched && btn_main_custom.contains(touch_pixel_x, touch_pixel_y));
  btn_main_control.press(isTouched && btn_main_control.contains(touch_pixel_x, touch_pixel_y));
  btn_main_settings.press(isTouched && btn_main_settings.contains(touch_pixel_x, touch_pixel_y));

  // switch to Coffee SubMenu
  if (btn_main_coffee.justReleased()) {
    btn_main_coffee.drawButton(false);
  }
  if (btn_main_coffee.justPressed()) {
    menuID = simpleCoffeeMenuID;
    btn_main_coffee.drawButton(true);
  }
  
  // Switch to Profile SubMenu
  if (btn_main_custom.justReleased()) {
    btn_main_custom.drawButton(false);
  }
  if (btn_main_custom.justPressed()) {
    menuID = customCoffeeMenuID;
    btn_main_custom.drawButton(true);
  }
  
  // Switch to Control SubMenu
  if (btn_main_control.justReleased()) {
    btn_main_control.drawButton(false);
  }
  if (btn_main_control.justPressed()) {
    menuID = controlMenuID;
    btn_main_control.drawButton(true);
  }

  // Switch to Settings SubMenu
  if (btn_main_settings.justReleased()) {
    btn_main_settings.drawButton(false);
  }
  if (btn_main_settings.justPressed()) {
    menuID = mainMenuID;
    btn_main_settings.drawButton(true);
  }

  return menuID;
}

}