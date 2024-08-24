#ifndef BUTTON_H
#define BUTTON_H
#include "Addresses_n_Macros.h"
#include <Arduino.h>
class Button {
private:
  int pin;
public:
  Button(int uPin) {
    pin = uPin;
  }
  void init() {
    set_pin_INPUT_pD(pin);
  }
  int getPinVal() {
    int pressed = check_pin_INPUT_state_pB(8);
    return pressed;
  }
};
#endif