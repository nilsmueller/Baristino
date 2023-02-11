#include "LCD.h"

namespace LCD {
int cProgBarX = 40;
int cProgBarY = 250;
int cProgBarW = LCD_WIDTH - 2*cProgBarX;    // total width of progbar (with bounds)
int cProgBarH = 40;                     // total height of progbar (with bounds)

int cProgBarWInner = cProgBarW - 4;     // inner width of progbar (without bounds)
int cProgBarHInner = cProgBarH - 4;     // inner height of progbar (without bounds)
int cProgBarRem = cProgBarWInner % 3;

int cProgBarW1 = (int)floor((cProgBarWInner + cProgBarRem) / 3);
int cProgBarW2 = cProgBarW1;
int cProgBarW3 = cProgBarWInner - cProgBarW1 - cProgBarW2;

uint16_t makeCoffeeScreenID = 11;

void drawMakeCoffeeScreen() {
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
    tft.print("Temp.  [C]");  

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 9*LCD_PAD + 20);
    tft.print("Water [mL]");  

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 16*LCD_PAD + 20);
    tft.print("Coffee [g]");


    tft.drawRect(cProgBarX, cProgBarY, cProgBarW, cProgBarH, myWHITE);
    tft.drawRect(cProgBarX+1, cProgBarY+1, cProgBarW-2, cProgBarH-2, myWHITE);
}


uint16_t updateMakeCoffeeScreen(double setTemp, double setVol, double setQty, double curTemp, double curVol, double curQty) {

    double p1 = max(0, min(1, setTemp - abs(setTemp - curTemp) / setTemp));
    //prog = 1 - max(0, min(1, abs((target_temp - current_temp) / (target_temp - start_temp))))
    double p2 = max(0, min(1, curVol / setVol));
    double p3 = max(0, min(1, curQty / setQty));

    // temperature
    tft.setTextSize(3);
    tft.setTextColor(myORANGE, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - 10, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD + 20);
    tft.print(curTemp);
    tft.print(" / ");
    tft.print((int)setTemp);

    // volume
    tft.setTextSize(3);
    tft.setTextColor(myBLUE, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - 10, LCD_MAIN_ORIGIN_Y + 9*LCD_PAD + 20);
    tft.print(curVol);
    tft.print(" / ");
    tft.print((int)setVol);

    // quantity
    tft.setTextSize(3);
    tft.setTextColor(myYELLOW, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - 10, LCD_MAIN_ORIGIN_Y + 15*LCD_PAD + 20);
    tft.print(curQty);
    tft.print(" / ");
    tft.print((int)setQty);


    int w1 = (int)(p1 * cProgBarW1) - 2;
    int w2 = int(p2 * cProgBarW2);
    int w3 = int(p3 * cProgBarW3);
    
    int x1 = cProgBarX + 2;
    int x2 = x1 + w1 + 1;
    int x3 = x2 + w2 + 1;

    // with boundaries
    /*
    tft.drawRect(x1, cProgBarY+2, w1, cProgBarHInner, (p1 < 1) ? myDARKORANGE : myDARKGREEN);
    tft.drawRect(x2, cProgBarY+2, w2, cProgBarHInner, (p2 < 1) ? myDARKBLUE2 : myDARKGREEN);
    tft.drawRect(x3, cProgBarY+2, w3, cProgBarHInner, (p3 < 1) ? myDARKYELLOW : myDARKGREEN);
    tft.drawRect(x1+1, cProgBarY+3, w1-2, cProgBarHInner-2, (p1 < 1) ? myDARKORANGE : myDARKGREEN);
    tft.drawRect(x2+1, cProgBarY+3, w2-2, cProgBarHInner-2, (p2 < 1) ? myDARKBLUE2 : myDARKGREEN);
    tft.drawRect(x3+1, cProgBarY+3, w3-2, cProgBarHInner-2, (p3 < 1) ? myDARKYELLOW : myDARKGREEN);

    tft.fillRect(x1+2, cProgBarY+4, w1-3, cProgBarHInner-3, (p1 < 1) ? myORANGE : myGREEN);
    tft.fillRect(x2+2, cProgBarY+4, w2-3, cProgBarHInner-3, (p2 < 1) ? myBLUE : myGREEN);
    tft.fillRect(x3+2, cProgBarY+4, w3-3, cProgBarHInner-3, (p3 < 1) ? myYELLOW : myGREEN);
    */

    // without Boundaries
    tft.fillRect(x1, cProgBarY+2, w1, cProgBarHInner, (p1 < 1) ? myDARKORANGE : myDARKGREEN);
    tft.fillRect(x2, cProgBarY+2, w2, cProgBarHInner, (p2 < 1) ? myDARKBLUE2 : myDARKGREEN);
    tft.fillRect(x3, cProgBarY+2, w3, cProgBarHInner, (p3 < 1) ? myDARKYELLOW : myDARKGREEN);

    return makeCoffeeScreenID;
}
}