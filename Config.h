#ifndef CONFIG_H
#define CONFIG_H

#include "Display.h"
#include "MenuNavigate.h"

namespace config {
uint8_t buzzerPin;

enum ONOFFTYPE {ON, OFF};
enum TYPES {STRING, INT, FLOAT, ONOFF};

class ConfigItem : public MenuItemBase {
  public:
    ConfigItem(String title) : MenuItemBase(title) {};
    ConfigItem(String title, bool backItem) : MenuItemBase(title, backItem) {};
    ConfigItem(String title, bool backItem, bool titleDisplay) : MenuItemBase(title, backItem, titleDisplay) {};
    ConfigItem(String title, OnSelectFunk* onSelect) : MenuItemBase(title, onSelect) {};

    ConfigItem(String title, String dataPath) : MenuItemBase(title), dataPath(dataPath) {};
    ConfigItem(String title, String dataPath, OnSelectFunk* onSelect) : MenuItemBase(title, onSelect), dataPath(dataPath) {};
    ConfigItem(String title, ONOFFTYPE onOffData, OnSelectFunk* onSelect) : MenuItemBase(title, onSelect), onOffData(onOffData) {type = ONOFF;};
    
    String dataPath;
    String stringData;
    int intData;
    float floatData;
    uint8_t floatPrec;
    ONOFFTYPE onOffData = OFF;
    TYPES type = STRING;
    
    String GetDataAsString() {
      switch (type) {
        case STRING:
          return stringData;
        case INT:
          return String(intData);
        case FLOAT:
          return String(floatData, floatPrec);
        case ONOFF:
          return onOffData == ON ? "ON" : "OFF";
        default:
          return "";
      }
    }
    
    String FormatItem(uint8_t dataCol) {
      String str;
      String data = GetDataAsString();
      if (data.length() == 0)
        str = title;
      else {
        str = (title + "                    ").substring(0, dataCol - 1);
        str += data;
      }
      return str;
    }
};

class ConfigDisplay : public MenuDisplayPCF8574 {
};

ConfigDisplay disp;
ConfigItem menu("MAIN MENU");
MenuNavigate nav(&menu, &disp);


bool soundOn = true;
bool backlightOn = true;

void SoundSelect(MenuItemBase* menuItem) {
  soundOn = !soundOn;
  nav.setSound(buzzerPin, soundOn);
  
  ConfigItem* item = static_cast<ConfigItem*>(menuItem);
  item->onOffData = soundOn ? ON : OFF;
  disp.DrawField(item);
}

void BacklightSelect(MenuItemBase* menuItem) {
  backlightOn = !backlightOn;
  disp.setBacklight(backlightOn);
  ConfigItem* item = static_cast<ConfigItem*>(menuItem);
  item->onOffData = backlightOn ? ON : OFF;
  disp.DrawField(item);
}


ConfigItem* newSettingItem(String title) {
  ConfigItem* item = new ConfigItem(title);
  item->subItems.push_back(new ConfigItem("<<BACK", true));
  item->subItems.push_back(new ConfigItem("sound", soundOn ? ON : OFF, SoundSelect)); //on/off
  item->subItems.push_back(new ConfigItem("backlight", backlightOn ? ON : OFF, BacklightSelect)); //on/off
  item->subItems.push_back(new ConfigItem("wifi mode"));          //enum AP/STA
  item->subItems.push_back(new ConfigItem("<<BACK", true));
  return item;
}
ConfigItem* newOnOffItem(String title) {
  ConfigItem* item = new ConfigItem(title);
  item->subItems.push_back(new ConfigItem("<<BACK", true));
  item->subItems.push_back(new ConfigItem("sensor"));    //enum T1/T2/T3/T4
  item->subItems.push_back(new ConfigItem("oper"));      //enum >/<
  item->subItems.push_back(new ConfigItem("temp"));      //num -100 .. +150
  item->subItems.push_back(new ConfigItem("time MM"));   //num 0 .. 240
  item->subItems.push_back(new ConfigItem("time SS"));   //num 0 .. 60
  item->subItems.push_back(new ConfigItem("<<BACK", true));
  return item;
}
ConfigItem* newRelayItem(String title) {
  ConfigItem* item = new ConfigItem(title);
  item->subItems.push_back(new ConfigItem("<<BACK", true));
  item->subItems.push_back(new ConfigItem("Mode"));       //enum Auto/On/Off
  item->subItems.push_back(newOnOffItem("ON rule"));
  item->subItems.push_back(newOnOffItem("OFF rule"));
  item->subItems.push_back(new ConfigItem("Atm Press")); //num +/-NN.NN
  item->subItems.push_back(new ConfigItem("<<BACK", true));
  return item;
}

void buildMenu(ConfigItem& menu) {
//  menu.subItems.push_back(new ConfigItem("Start/Stop"));
  menu.subItems.push_back(newRelayItem("RELAY 1"));
  menu.subItems.push_back(newRelayItem("RELAY 2"));
  menu.subItems.push_back(newRelayItem("RELAY 3"));
  menu.subItems.push_back(newRelayItem("RELAY 4"));
  menu.subItems.push_back(newSettingItem("SETTINGS"));
  menu.subItems.push_back(new ConfigItem("SAVE and exit"));
  menu.subItems.push_back(new ConfigItem("EXIT w/o save"));
}

;
void setup(uint8_t buzPin) {
  buzzerPin = buzPin;
  buildMenu(menu);
}

void begin() {
  disp.clear();
  nav.begin();
  nav.setSound(buzzerPin, soundOn);
}

void onEncLeft(ESPRotary& rotary) {
  nav.up();
}
void onEncRight(ESPRotary& rotary) {
  nav.down();
}
void buttonPressed(Button2& btn) {
  nav.select();
}


} //namespace
#endif
