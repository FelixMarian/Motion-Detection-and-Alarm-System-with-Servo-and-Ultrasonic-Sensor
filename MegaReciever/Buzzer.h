#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>

class Buzzer {
private:
  int buzzerPin;
public:
  Buzzer(int buzzerPin_c) {
    buzzerPin = buzzerPin_c;
    pinMode(buzzerPin, OUTPUT);
  }
  // Activate the buzzer with a 5 KHz tune
  void AlarmOn(){ 
    tone(buzzerPin,5000);
  }
  // Deactivate the buzzer
  void AlarmOff(){
    noTone(buzzerPin);
  }
};

#endif