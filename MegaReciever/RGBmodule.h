#ifndef RGBMODULE_H
#define RGBMODULE_H
#include <Arduino.h>


class RGBmodule {
private:
  int R_pin, G_pin, B_pin; //R-Red, G-Green, B-Blue color pins
public:
  RGBmodule(int R_pin_c, int G_pin_c, int B_pin_c) {
    R_pin = R_pin_c;
    G_pin = G_pin_c;
    B_pin = B_pin_c;
    pinMode(R_pin, OUTPUT);
    pinMode(G_pin, OUTPUT);
    pinMode(B_pin, OUTPUT);
  }
  void LOW_on_pins() {
    digitalWrite(R_pin, LOW);
    digitalWrite(G_pin, LOW);
    digitalWrite(B_pin, LOW);
  }
  void R_on() {
    LOW_on_pins();
    digitalWrite(R_pin, HIGH);
  }
  void G_on() {
    LOW_on_pins();
    digitalWrite(G_pin, HIGH);
  }
  //Yellow color as a combination of Red and Green
  void Y_on() {
    LOW_on_pins();
    digitalWrite(G_pin, HIGH);
    digitalWrite(R_pin, HIGH);
  }
};

#endif