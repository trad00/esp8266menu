#ifndef MENU_NAVIGATE_H
#define MENU_NAVIGATE_H

#include <vector> 

class IDisplay {
  public:
    virtual int getRowCount() = 0;
    virtual void startDraw() = 0;
    virtual void drawTitle(String title) = 0;
    virtual void drawLine(String title) = 0;
    virtual void selectLine(int line) = 0;
};

class MenuItemBase {
  public:
    MenuItemBase(){};
    MenuItemBase(String title) : title(title){};
    MenuItemBase(String title, bool backItem) : title(title), backItem(backItem){};
    MenuItemBase(String title, bool backItem, bool titleDisplay) : title(title), backItem(backItem), titleDisplay(titleDisplay){};
    String title;
    bool titleDisplay = false;
    bool backItem = false;
    std::vector<MenuItemBase*> subItems;
  friend class MenuNavigate;
};

class MenuNavigate {
  public:
    MenuNavigate(MenuItemBase* menu, IDisplay* display)
    : rootMenu(menu), display(display) {
      current.menu = menu;
      current.frameLines = display->getRowCount();
    }
    void begin();
    void end();
    void menuDraw(bool redraw);
    void menuEnter(MenuItemBase* menu);
    void menuExit();
    void menuUp();
    void menuDn();
    void menuSelect();
    
  private:
    IDisplay* display;
    MenuItemBase* rootMenu;
    
    struct CurrentMenuData {
      MenuItemBase* menu;
      int startLine = 0;
      int frameLines;
      int line = 0;
      int frameOffset = 0;
      int menuPos;
    } current;
    
    std::vector<CurrentMenuData> breadcrumbs;
};

void MenuNavigate::begin() {
  breadcrumbs.clear();
  menuEnter(rootMenu);
};


void MenuNavigate::menuDraw(bool redraw) {
  if (redraw){
    display->startDraw();
    if (current.menu->titleDisplay) {
      display->drawTitle(current.menu->title);
    }
    for (int i = current.frameOffset;  i < current.frameOffset + current.frameLines; i++) {
      if (i < current.menu->subItems.size()) {
        display->drawLine(current.menu->subItems[i]->title);
      }
    }
  }
  display->selectLine(current.line);
}

void MenuNavigate::menuEnter(MenuItemBase* menu) {
  if (menu->subItems.size() == 0)
    return;
  
  breadcrumbs.push_back(current);
  
  current.menu = menu;
  current.startLine = current.menu->titleDisplay ? 1 : 0;
  current.frameLines = display->getRowCount() - current.startLine;
  if (current.menu->subItems.size() < current.frameLines)
    current.frameLines = current.menu->subItems.size();
  
  current.line = current.startLine;
  current.frameOffset = 0;
  current.menuPos = current.frameOffset + current.line;
  if (current.menu->titleDisplay)
    current.menuPos--;

  menuDraw(true);
}

void MenuNavigate::menuExit() {
  current = breadcrumbs.back();
  breadcrumbs.pop_back();
  menuDraw(true);
}

void MenuNavigate::menuUp() {
  if (current.line > current.startLine) {
    //cursor in frame move
    current.line--;
    menuDraw(false);
  } else if (current.frameOffset > 0) {
    //frame offset move
    current.frameOffset--;
    menuDraw(true);
  }
  current.menuPos = current.frameOffset + current.line;
  if (current.menu->titleDisplay)
    current.menuPos--;
}

void MenuNavigate::menuDn() {
  if (current.line < current.startLine + current.frameLines - 1) {
    //cursor in frame move
    current.line++;
    menuDraw(false);
  } else if (current.frameOffset < current.menu->subItems.size() - current.frameLines) {
    //frame offset move
    current.frameOffset++;
    menuDraw(true);
  }
  current.menuPos = current.frameOffset + current.line;
  if (current.menu->titleDisplay)
    current.menuPos--;
}

void MenuNavigate::menuSelect() {
  MenuItemBase* subItem = current.menu->subItems[current.menuPos];
  if (subItem->subItems.size() > 0) {
    menuEnter(subItem);
  }
  else if (subItem->backItem){
    menuExit();
  }
//  else if (subItem->onSelect) {
//    subItem->onSelect();
//  }
}

#endif
