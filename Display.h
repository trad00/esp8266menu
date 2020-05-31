#ifndef DISPLAY_H
#define DISPLAY_H

#include "MenuNavigate.h"
#include <LiquidCrystal_PCF8574.h>

class DisplayPCF8574 : public IDisplay {
  #define LCD_COLS 20
  #define LCD_ROWS 4
  private:
    LiquidCrystal_PCF8574 lcd;
    byte curLine;
    byte prevLine;

  public:
    DisplayPCF8574() : lcd(0x27) {
      lcd.begin(LCD_COLS, LCD_ROWS);
      lcd.setBacklight(true);
      lcd.home();
      curLine = 0;
      prevLine = 0;
    }
    int getRowCount() {
      return LCD_ROWS;
    }
    int clear() {
      lcd.clear();
    }
    void startDraw() {
      lcd.clear();
      curLine = 0;
    }
    void drawTitle(String title) {
      int pos = (LCD_COLS - title.length()) / 2;
      if (pos < 0)
        pos = 0;
      lcd.setCursor(pos, curLine++);
      lcd.print(title);
    }
    void drawLine(String title) {
      lcd.setCursor(1, curLine++);
      lcd.print(title);
    }
    void selectLine(int line) {
      lcd.setCursor(0, prevLine);
      lcd.print(" ");
      lcd.setCursor(0, line);
      lcd.print(">");
      prevLine = line;
    }
};

#endif
