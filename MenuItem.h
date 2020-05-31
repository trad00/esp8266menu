#include "MenuNavigate.h"

class MenuItem : public MenuItemBase {
  public:
    MenuItem(String title) : MenuItemBase(title){};
    MenuItem(String title, bool backItem) : MenuItemBase(title, backItem){};
    MenuItem(String title, bool backItem, bool titleDisplay) : MenuItemBase(title, backItem, titleDisplay){};
};


MenuItem* newSettingItem(String title) {
  MenuItem* item = new MenuItem(title);
  item->subItems.push_back(new MenuItem("<<BACK", true));
  item->subItems.push_back(new MenuItem("sound"));
  item->subItems.push_back(new MenuItem("backlight"));
  item->subItems.push_back(new MenuItem("wifi mode"));
  item->subItems.push_back(new MenuItem("<<BACK", true));
  return item;
}
MenuItem* newOnOffItem(String title) {
  MenuItem* item = new MenuItem(title);
  item->subItems.push_back(new MenuItem("<<BACK", true));
  item->subItems.push_back(new MenuItem("sensor"));
  item->subItems.push_back(new MenuItem("oper"));
  item->subItems.push_back(new MenuItem("temp"));
  item->subItems.push_back(new MenuItem("time"));
  item->subItems.push_back(new MenuItem("<<BACK", true));
  return item;
}
MenuItem* newRelayItem(String title) {
  MenuItem* item = new MenuItem(title);
  item->subItems.push_back(new MenuItem("<<BACK", true));
  item->subItems.push_back(new MenuItem("Mode"));
  item->subItems.push_back(newOnOffItem("ON rule"));
  item->subItems.push_back(newOnOffItem("OFF rule"));
  item->subItems.push_back(new MenuItem("Atm Press"));
  item->subItems.push_back(new MenuItem("<<BACK", true));
  return item;
}

void buildMenu(MenuItem& menu) {
  menu.subItems.push_back(new MenuItem("Start/Stop"));
  menu.subItems.push_back(newRelayItem("RELAY 1"));
  menu.subItems.push_back(newRelayItem("RELAY 2"));
  menu.subItems.push_back(newRelayItem("RELAY 3"));
  menu.subItems.push_back(newRelayItem("RELAY 4"));
  menu.subItems.push_back(newSettingItem("SETTINGS"));
  menu.subItems.push_back(new MenuItem("SAVE and exit"));
  menu.subItems.push_back(new MenuItem("EXIT w/o save"));
}
