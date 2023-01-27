#include "LCD.h"

namespace LCD {

void drawWarmUpScreen() {
  tft.fillScreen(myBLACK);
  // complete rect
  tft.drawRect(1, 1, LCD_WIDTH - LCD_BORDER, LCD_HEIGHT - LCD_BORDER, myBLUE);
  tft.drawRect(2, 2, LCD_WIDTH - 2*LCD_BORDER, LCD_HEIGHT - 2*LCD_BORDER, myBLUE);

  tft.drawRect(LCD_PAD + LCD_BORDER, LCD_PAD + LCD_BORDER, LCD_WIDTH - 2*(LCD_BORDER + LCD_PAD), LCD_HEIGHT - 2*(LCD_BORDER + LCD_PAD), myBLUE);
  tft.drawRect(LCD_PAD + LCD_BORDER + 1, LCD_PAD + LCD_BORDER + 1, LCD_WIDTH - 2*(LCD_BORDER + LCD_PAD) - 2, LCD_HEIGHT - 2*(LCD_BORDER + LCD_PAD) - 2, myBLUE);
  
  int16_t  x1, y1;
  uint16_t w, h;

  tft.getTextBounds("Heating Up", LCD_WIDTH/2, LCD_HEIGHT/3, &x1, &y1, &w, &h);
  tft.setCursor(x1 - w/2, y1 - h/2);
  tft.setTextColor(myWHITE);
  tft.setTextSize(3);
  tft.print("Heating Up");

  int width_progbar = 300;

  tft.drawRect(LCD_WIDTH/2 - width_progbar/2 - 50, LCD_HEIGHT/3 + 50, width_progbar, 40, myWHITE);
  tft.drawRect(LCD_WIDTH/2 - width_progbar/2 - 50 + 1, LCD_HEIGHT/3 + 50 + 1, width_progbar - 2, 38, myWHITE);
  tft.setCursor(LCD_WIDTH/2 - width_progbar/2 - 50 + width_progbar + 20, LCD_HEIGHT/3 + 50 + 10);
  tft.print("0   %");

}


void updateWarmUpScreen(double percentage) {

  int perc = int(percentage * 100);
  int percentage_px = (int)(296 * percentage);

  int x0 = LCD_WIDTH/2 - 200 + 2;
  int y0 = LCD_HEIGHT/3 + 52;
  tft.fillRect(x0, y0, percentage_px, 36, myGREEN);

  tft.setTextColor(myWHITE, myBLACK);  
  tft.setTextSize(3);
  tft.setCursor(LCD_WIDTH/2 - 300/2 - 50 + 300 + 20, LCD_HEIGHT/3 + 50 + 10);
  

  tft.print(perc);
  tft.print(" %  ");
}

}