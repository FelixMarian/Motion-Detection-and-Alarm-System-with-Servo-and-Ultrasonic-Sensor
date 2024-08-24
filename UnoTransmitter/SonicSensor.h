#ifndef SONICSENSOR_H
#define SONICSENSOR_H

#include "Addresses_n_Macros.h"
#include <Arduino.h>

class sonicSensor {
private:
  int triggerPin;
  int echoPin;
  unsigned long triggerStartTime;
  bool triggerActive;

public:
  sonicSensor(int triggerPinC, int echoPinC) {
    triggerPin = triggerPinC;
    echoPin = echoPinC;
    // Initialize pins
    set_pin_OUTPUT_pD(triggerPin);
    set_pin_INPUT_pD(echoPin);
    write_pin_LOW_pD(triggerPin);  
  }

  int getDistance() {
    // Send a 2-microsecond pulse on the trigger pin to start measurement
    write_pin_LOW_pD(triggerPin);
    delayMicroseconds(2);
    write_pin_HIGH_pD(triggerPin);
    delayMicroseconds(10);
    write_pin_LOW_pD(triggerPin);

    // Measure the pulse duration on the echo pin
    long duration = pulseIn(echoPin, HIGH);  // Pulse duration in microseconds
    
    // Return -1 to indicate an error if no pulse is received
    if (duration == 0) return -1;

    // Calculate distance in centimeters
    int distance = (duration / 2.0) * 0.0343;

    // Check if the distance is within the acceptable range
    if (distance >= 2 && distance <= 450) {
      return distance;
    } else {
      return -1;
    }
  }
};

#endif
