#ifndef DISPLAY_H
#define DISPLAY_H

#include "MenuNavigate.h"
#include <LiquidCrystal_PCF8574.h>

class MenuDisplayPCF8574 : public IMenuDisplay {
  #define LCD_COLS 20
  #define LCD_ROWS 4
  #define DATA_FIELD_COL 15
  private:
    LiquidCrystal_PCF8574 lcd;
    uint8_t curLine;
    uint8_t prevLine;

  public:
    MenuDisplayPCF8574() : lcd(0x27) {
      lcd.begin(LCD_COLS, LCD_ROWS);
      lcd.setBacklight(true);
      lcd.home();
      curLine = 0;
      prevLine = 0;
    }
    void setBacklight(int brightness) {
      lcd.setBacklight(brightness);
    }
    uint8_t getRowCount() {
      return LCD_ROWS;
    }
    void clear() {
      lcd.clear();
    }
    void startDraw() {
      lcd.clear();
      curLine = 0;
    }
    void DrawTitle(MenuItemBase* item) {
      String title = item->GetTitle();
      uint8_t pos = (LCD_COLS - title.length()) / 2;
      if (pos < 0)
        pos = 0;
      lcd.setCursor(pos, curLine++);
      lcd.print(title);
    }
    void DrawItem(MenuItemBase* item) {
      String str = item->FormatItem(DATA_FIELD_COL).substring(0, LCD_COLS-2);
      lcd.setCursor(1, curLine++);
      lcd.print(str);
    }
    void DrawField(MenuItemBase* item) {
      String str = (item->GetDataAsString()+"     ").substring(0, LCD_COLS - DATA_FIELD_COL - 2);
      lcd.setCursor(DATA_FIELD_COL, item->displayLine);
      lcd.print(str);
    }
    void selectLine(uint8_t line) {
      lcd.setCursor(0, prevLine);
      lcd.print(" ");
      lcd.setCursor(0, line);
      lcd.print(">");
      prevLine = line;
    }
};

#endif
