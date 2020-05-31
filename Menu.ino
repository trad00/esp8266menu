#include <Button.h>
#include <ESPRotary.h>

#include "Display.h"
#include "MenuNavigate.h"

#define ENC_BTN D5
#define ENC_A D6
#define ENC_B D7

DisplayPCF8574 disp;
MenuItem menu;

MenuNavigate nav(&menu, &disp);
Button button(ENC_BTN);
ESPRotary rotary(ENC_A, ENC_B, 4);

void buttonPressed() {
  nav.menuSelect();
}
void onEncRight(ESPRotary& rotary) {
  nav.menuDn();
}
void onEncLeft(ESPRotary& rotary) {
  nav.menuUp();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_BTN, INPUT_PULLUP);

  buildMenu();

  disp.clear();
  
  button.begin();
  rotary.setRightRotationHandler(onEncRight);
  rotary.setLeftRotationHandler(onEncLeft);
  
  nav.begin();
}

void loop() {
  rotary.loop();
  if (button.toggled())
    if (button.read() == Button::PRESSED)
      buttonPressed();
}

class MenuItem : public MenuItemBase {
};

MenuItem* newSettingItem(String title) {
  MenuItem* item = new MenuItem(title, true);
  item->subItems.push_back(new MenuItem("<<BACK", false, true));
  item->subItems.push_back(new MenuItem("sound"));
  item->subItems.push_back(new MenuItem("backlight"));
  item->subItems.push_back(new MenuItem("wifi mode"));
  item->subItems.push_back(new MenuItem("<<BACK", false, true));
  return item;
}
MenuItem* newOnOffItem(String title) {
  MenuItem* item = new MenuItem(title, true);
  item->subItems.push_back(new MenuItem("<<BACK", false, true));
  item->subItems.push_back(new MenuItem("sensor"));
  item->subItems.push_back(new MenuItem("oper"));
  item->subItems.push_back(new MenuItem("temp"));
  item->subItems.push_back(new MenuItem("time"));
  item->subItems.push_back(new MenuItem("<<BACK", false, true));
  return item;
}
MenuItem* newRelayItem(String title) {
  MenuItem* item = new MenuItem(title, true);
  item->subItems.push_back(new MenuItem("<<BACK", false, true));
  item->subItems.push_back(new MenuItem("Mode"));
  item->subItems.push_back(newOnOffItem("ON rule"));
  item->subItems.push_back(newOnOffItem("OFF rule"));
  item->subItems.push_back(new MenuItem("Atm Press"));
  item->subItems.push_back(new MenuItem("<<BACK", false, true));
  return item;
}
void buildMenu() {
  menu.title = "MAIN MENU";
  menu.titleDisplay = false;
  menu.subItems.push_back(new MenuItem("Start/Stop"));
  menu.subItems.push_back(newRelayItem("RELAY 1"));
  menu.subItems.push_back(newRelayItem("RELAY 2"));
  menu.subItems.push_back(newRelayItem("RELAY 3"));
  menu.subItems.push_back(newRelayItem("RELAY 4"));
  menu.subItems.push_back(newSettingItem("SETTINGS"));
  menu.subItems.push_back(new MenuItem("SAVE and exit"));
  menu.subItems.push_back(new MenuItem("EXIT w/o save"));
}
