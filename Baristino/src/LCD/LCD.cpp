#include "LCD.h"

namespace LCD {

// Frame
const int LCD_WIDTH = 480;
const int LCD_HEIGHT = 320;
const int LCD_PAD = 6;
const int LCD_BORDER = 2;

// TopBar
const int LCD_TOPBAR_WIDTH = LCD_WIDTH - 2*LCD_PAD - 4*LCD_BORDER;
const int LCD_TOPBAR_HEIGHT = 60 + 2 * LCD_PAD;
const int LCD_TOPBAR_OFFSET_X = 10;
const int LCD_TOPBAR_OFFSET_Y = 10;

// MainBody
const int LCD_MAIN_ORIGIN_X = 2*LCD_BORDER + 2*LCD_PAD;
const int LCD_MAIN_ORIGIN_Y = 3*LCD_BORDER + LCD_TOPBAR_HEIGHT + 2*LCD_PAD;
const int LCD_MAIN_WIDTH = LCD_TOPBAR_WIDTH;
const int LCD_MAIN_HEIGHT = LCD_HEIGHT - LCD_TOPBAR_HEIGHT - 3*LCD_PAD - 6*LCD_BORDER;
const int LCD_MAIN_HALFWIDTH = LCD_MAIN_WIDTH/2 - LCD_PAD/2 + LCD_BORDER;

// Touch Coodinates (rotation 1 = 'landscape')
int touch_pixel_x, touch_pixel_y;
const int TS_LEFT = 955;
const int TS_RT = 89;
const int TS_TOP = 913;
const int TS_BOT = 110;


//TouchScreen ts = TouchScreen(pindef::LCD_XP, pindef::LCD_YP, pindef::LCD_XM, pindef::LCD_YM, 300);
TouchScreen ts = TouchScreen(pindef::LCD_XP, pindef::LCD_YP, pindef::LCD_XM, pindef::LCD_YM, 300);
MCUFRIEND_kbv tft;


void initialize() {
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486; // write-only shield
  tft.begin(ID);
  if (LCD_ROTATE) {
    tft.setRotation(3);
  }
  else {
    tft.setRotation(1);
  }            
}


void drawEmptyFrame() {
  tft.fillScreen(myBLACK);
  // complete rect
  tft.drawRect(1, 1, LCD_WIDTH - LCD_BORDER, LCD_HEIGHT - LCD_BORDER, myBLUE);
  tft.drawRect(2, 2, LCD_WIDTH - 2*LCD_BORDER, LCD_HEIGHT - 2*LCD_BORDER, myBLUE);
}

void drawTopBar() {
  // topbar rect
  tft.drawRect(LCD_PAD + LCD_BORDER, LCD_PAD + LCD_BORDER, LCD_WIDTH - 2*(LCD_BORDER + LCD_PAD), LCD_TOPBAR_HEIGHT + 2*LCD_BORDER, myBLUE);
  tft.drawRect(LCD_PAD + LCD_BORDER + 1, LCD_PAD + LCD_BORDER + 1, LCD_WIDTH - 2*(LCD_BORDER + LCD_PAD) - 2, LCD_TOPBAR_HEIGHT + 2*LCD_BORDER - 2, myBLUE);
  
}

void drawMainBody() {
  tft.drawRect(LCD_PAD + LCD_BORDER, LCD_TOPBAR_HEIGHT + 2*LCD_PAD + 3*LCD_BORDER, LCD_WIDTH - 2*(LCD_BORDER + LCD_PAD) , LCD_HEIGHT - LCD_TOPBAR_HEIGHT - 4*LCD_BORDER - 3*LCD_PAD, myBLUE);
  tft.drawRect(LCD_PAD + LCD_BORDER + 1, LCD_TOPBAR_HEIGHT + 2*LCD_PAD + 3*LCD_BORDER + 1, LCD_WIDTH - 2*(LCD_BORDER + LCD_PAD) - 2, LCD_HEIGHT - LCD_TOPBAR_HEIGHT - 4*LCD_BORDER - 3*LCD_PAD - 2, myBLUE);
}

void drawMainBodyDouble() {
  tft.drawRect(LCD_PAD + LCD_BORDER, LCD_TOPBAR_HEIGHT + 2*LCD_PAD + 3*LCD_BORDER, LCD_MAIN_HALFWIDTH, LCD_HEIGHT - LCD_TOPBAR_HEIGHT - 4*LCD_BORDER - 3*LCD_PAD, myBLUE);
  tft.drawRect(LCD_PAD + LCD_BORDER + 1, LCD_TOPBAR_HEIGHT + 2*LCD_PAD + 3*LCD_BORDER + 1, LCD_MAIN_HALFWIDTH - 2, LCD_HEIGHT - LCD_TOPBAR_HEIGHT - 4*LCD_BORDER - 3*LCD_PAD - 2, myBLUE);

  tft.drawRect(LCD_WIDTH/2 + LCD_PAD/2, LCD_TOPBAR_HEIGHT + 2*LCD_PAD + 3*LCD_BORDER, LCD_MAIN_HALFWIDTH, LCD_HEIGHT - LCD_TOPBAR_HEIGHT - 4*LCD_BORDER - 3*LCD_PAD, myBLUE);
  tft.drawRect(LCD_WIDTH/2 + LCD_PAD/2 + 1, LCD_TOPBAR_HEIGHT + 2*LCD_PAD + 3*LCD_BORDER + 1, LCD_MAIN_HALFWIDTH - 2, LCD_HEIGHT - LCD_TOPBAR_HEIGHT - 4*LCD_BORDER - 3*LCD_PAD - 2, myBLUE);
}

void drawEmptyScreen() {
  drawEmptyFrame();
  drawTopBar();
  drawMainBody();
}

void drawEmptyScreenDouble() {
  drawEmptyFrame();
  drawTopBar();
  drawMainBodyDouble();
}


void sleep() {
  tft.fillScreen(0x0000);
}


void drawErrorScreen(ErrorCode code) {
  tft.fillRect(100, 100, LCD_WIDTH-100, LCD_HEIGHT-100, myBLACK);
  tft.drawRect(101, 101, LCD_WIDTH-102, LCD_HEIGHT-102, myBLUE);
  tft.drawRect(102, 102, LCD_WIDTH-104, LCD_HEIGHT-104, myBLUE);
}

bool getTouchCoord(void) {
    TSPoint p = ts.getPoint();
    pinMode(pindef::LCD_YP, OUTPUT);
    pinMode(pindef::LCD_XM, OUTPUT);
    digitalWrite(pindef::LCD_YP, HIGH);
    digitalWrite(pindef::LCD_XM, HIGH);

    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        if (LCD_ROTATE) {
          touch_pixel_x = map(p.y, TS_RT, TS_LEFT, 0, tft.width());
          touch_pixel_y = map(p.x, TS_BOT, TS_TOP, 0, tft.height());
        }
        else {
          touch_pixel_x = map(p.y, TS_LEFT, TS_RT, 0, tft.width());
          touch_pixel_y = map(p.x, TS_TOP, TS_BOT, 0, tft.height());
        }
    }
    return pressed;
}


}