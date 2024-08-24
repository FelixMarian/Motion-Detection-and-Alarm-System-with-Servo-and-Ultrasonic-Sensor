#ifndef SERVOM_H
#define SERVOM_H

#include <Servo.h>
#include "checkTimePassing.h"

class ServoMot {
private:
  Servo myServoM;
  int pin;
  int currAngle;
  unsigned long lastServoCheck;
  bool forward;

public:
  // Constructor for Servo
  ServoMot(int ServoPin) {
    pin = ServoPin;
    currAngle = 0;
    lastServoCheck = 0;
    forward = true;
  }

  // Initialize the Servo
  void begin() {
    myServoM.attach(pin);
    myServoM.write(currAngle);
  }

  /* Function to rotate the servo by 'step' degrees every 'timeP' milliseconds
     The servo moves from 0° to 180° and then back from 180° to 0° */
  void rotateCycle(int step, int timeP, int &poz) {
    unsigned long currentMillis = millis();
    if (hasTimePassed(lastServoCheck, currentMillis, timeP)) {
      lastServoCheck = currentMillis;
      if (forward) {
        currAngle += step;
        poz += step;
        if (currAngle >= 180) {
          currAngle = 180;
          forward = false;
        }
      } else {
        currAngle -= step;
        poz -= step;
        if (currAngle <= 0) {
          currAngle = 0;
          forward = true;
        }
      }
      myServoM.write(currAngle);
    }
  }

  // Reset the servo to the default position (0°)
  void setDefault() {
    myServoM.write(0);
    currAngle = 0;
  }
};

#endif
