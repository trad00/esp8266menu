#include <Button.h>
#include <ESPRotary.h>

#include "Display.h"
#include "MenuNavigate.h"
#include "MenuItem.h"

#define ENC_BTN D5
#define ENC_A D6
#define ENC_B D7

DisplayPCF8574 disp;
MenuItem menu("MAIN MENU");

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

  buildMenu(menu);

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
