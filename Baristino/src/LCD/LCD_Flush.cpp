#include "LCD.h"

Adafruit_GFX_Button btn_flush;
int btn_flush_width = 200;
int btn_flush_height = 100;

namespace LCD {


void drawFlushScreen() {
    tft.fillScreen(myBLACK);
    // complete rect
    tft.drawRect(1, 1, LCD_WIDTH - LCD_BORDER, LCD_HEIGHT - LCD_BORDER, myBLUE);
    tft.drawRect(2, 2, LCD_WIDTH - 2*LCD_BORDER, LCD_HEIGHT - 2*LCD_BORDER, myBLUE);

    tft.drawRect(LCD_PAD + LCD_BORDER, LCD_PAD + LCD_BORDER, LCD_WIDTH - 2*(LCD_BORDER + LCD_PAD), LCD_HEIGHT - 2*(LCD_BORDER + LCD_PAD), myBLUE);
    tft.drawRect(LCD_PAD + LCD_BORDER + 1, LCD_PAD + LCD_BORDER + 1, LCD_WIDTH - 2*(LCD_BORDER + LCD_PAD) - 2, LCD_HEIGHT - 2*(LCD_BORDER + LCD_PAD) - 2, myBLUE);

    char btnLabel[1][10] = {"Flush"};
    btn_flush.initButtonUL(&tft, LCD_WIDTH - btn_flush_width/2, LCD_HEIGHT - btn_flush_height/2, btn_flush_width, btn_flush_height, myBLUE, myYELLOW, myWHITE, btnLabel[0], 4);
    btn_flush.drawButton(false);
}

uint16_t updateFlushScreen() {

    bool isTouched = getTouchCoord();
    uint8_t menuID = flushMenuID;
  
    btn_flush.press(isTouched && btn_flush.contains(touch_pixel_x, touch_pixel_y));

    // switch to Coffee SubMenu
    if (btn_flush.justReleased()) {
        btn_flush.drawButton(false);
    }
    if (btn_flush.justPressed()) {
        menuID = simpleCoffeeMenuID;
        btn_flush.drawButton(true);
    }

    return menuID;
}

}