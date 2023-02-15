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

uint16_t makeCoffeeScreenID = 21;

char temp_cur_buffer[7];
char temp_disp[18];
char vol_cur_buffer[7];
char vol_disp[18];
char dose_cur_buffer[7];
char dose_disp[18];


void drawMakeCoffeeScreen(BrewParam *process) {
    drawEmptyScreen();
    
    // Topbar
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 2*LCD_PAD);
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);
    tft.print("Coffee in");
    tft.setCursor(LCD_TOPBAR_OFFSET_X + LCD_PAD + 135, LCD_TOPBAR_OFFSET_Y + 7*LCD_PAD);
    tft.print("process..");

    // Main 
    tft.setTextColor(myWHITE);
    tft.setTextSize(3);

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD + 20);
    tft.print("Coffee [g]");

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 9*LCD_PAD + 20);
    tft.print("Temp.  [C]");  

    tft.setCursor(LCD_MAIN_ORIGIN_X, LCD_MAIN_ORIGIN_Y + 16*LCD_PAD + 20);
    tft.print("Water [mL]");  

    // dose
    tft.setTextColor(myYELLOW, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - 20, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD + 20);
    tft.print((int)process->current_dose);
    tft.print(" / ");
    tft.print((int)process->set_dose);

    // temperature
    tft.setTextColor(myORANGE, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - 20, LCD_MAIN_ORIGIN_Y + 9*LCD_PAD + 20);
    tft.print((int)process->current_temperature);
    tft.print(" / ");
    tft.print((int)process->set_temperature);

    // volume
    tft.setTextColor(myBLUE, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - 20, LCD_MAIN_ORIGIN_Y + 16*LCD_PAD + 20);
    tft.print((int)process->current_volume);
    tft.print(" / ");
    tft.print((int)process->set_volume);


    tft.drawRect(cProgBarX, cProgBarY, cProgBarW, cProgBarH, myWHITE);
    tft.drawRect(cProgBarX+1, cProgBarY+1, cProgBarW-2, cProgBarH-2, myWHITE);
}


uint16_t updateMakeCoffeeScreen(BrewParam *process) {
    // quantity
    tft.setTextSize(3);
    tft.setTextColor(myYELLOW, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - 20, LCD_MAIN_ORIGIN_Y + 2*LCD_PAD + 20);
    dtostrf(process->current_dose, 5, 2, dose_cur_buffer);
    sprintf(dose_disp, "%s / %d", dose_cur_buffer, (int)process->set_dose);
    tft.print(dose_disp);

    // temperature
    tft.setTextSize(3);
    tft.setTextColor(myORANGE, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - 20, LCD_MAIN_ORIGIN_Y + 9*LCD_PAD + 20);
    dtostrf(process->current_temperature, 5, 2, temp_cur_buffer);
    sprintf(temp_disp, "%s / %d", temp_cur_buffer, (int)process->set_temperature);
    tft.print(temp_disp);


    // volume
    tft.setTextSize(3);
    tft.setTextColor(myBLUE, myBLACK);
    tft.setCursor(LCD_MAIN_ORIGIN_X + LCD_MAIN_WIDTH/2 - 20, LCD_MAIN_ORIGIN_Y + 16*LCD_PAD + 20);
    dtostrf(process->current_volume, 5, 2, vol_cur_buffer);
    sprintf(vol_disp, "%s / %d", vol_cur_buffer, (int)process->set_volume);
    tft.print(vol_disp);

    // dose
    double p1 = max(0, min(1, process->current_dose / process->set_dose));
    double val = abs((process->set_temperature - process->current_temperature) / (process->set_temperature - process->start_temperature));
    double p2 = 1 - max(0, min(1, val));
    double p3 = max(0, min(1, process->current_volume / process->set_volume));

    int w1 = (int)(p1 * cProgBarW1);
    int w2 = (int)(p2 * cProgBarW2);
    int w3 = (int)(p3 * cProgBarW3);
    
    int x1 = cProgBarX + 2;
    int x2 = x1 + w1;
    int x3 = x2 + w2;

    // without Boundaries
    tft.fillRect(x1, cProgBarY+2, w1, cProgBarHInner, (p1 < 1) ? myYELLOW: myDARKGREEN);
    tft.fillRect(x2, cProgBarY+2, w2, cProgBarHInner, (p2 < 1) ? myORANGE: myDARKGREEN);
    tft.fillRect(x3, cProgBarY+2, w3, cProgBarHInner, (p3 < 1) ? myBLUE : myDARKGREEN);
    tft.fillRect(x3+w3, cProgBarY+2, cProgBarWInner-w1-w2-w3, cProgBarHInner, myBLACK);

    return makeCoffeeScreenID;
}
}