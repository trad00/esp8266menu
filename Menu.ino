#include <Button2.h>
#include <ESPRotary.h>

#include "pins.h"
#include "pitches.h"
#include "Config.h"

enum MODES { MAIN, CONFIG };
MODES mode = CONFIG;

Button2 button(ENC_BTN);
ESPRotary rotary(ENC_A, ENC_B, 4);

void onEncLeft(ESPRotary& rotary) {
  if (mode == CONFIG)
    config::onEncLeft(rotary);
}
void onEncRight(ESPRotary& rotary) {
  if (mode == CONFIG)
    config::onEncRight(rotary);
}
void buttonPressed(Button2& btn) {
  if (mode == CONFIG)
    config::buttonPressed(btn);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_BTN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  button.setPressedHandler(buttonPressed);
  rotary.setRightRotationHandler(onEncRight);
  rotary.setLeftRotationHandler(onEncLeft);

  config::setup(BUZZER);
  if (mode == CONFIG)
    config::begin();

//  if (config::soundOn)
//    startMelodyPlay();
}

void loop() {
  rotary.loop();
  button.loop();
}
