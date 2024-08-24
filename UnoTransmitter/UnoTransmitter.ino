#include "ServoM.h"
#include <EEPROM.h>
#include "ProcessCommands.h"
#include "SonicSensor.h"
#include "checkTimePassing.h"
#include "button.h"
#include <SoftwareSerial.h>

// Initialize software serial, ultrasonic sensor, servo motor, and button
SoftwareSerial SerialUART(3, 4);  
sonicSensor HC_SR04(6, 7);       
ServoMot servoM(5);              
Button Butt(8);                  

int step, timeP, vectorIndex = 0, arrayLength, poz = 0, newScan = 0, distance;
unsigned long lastCommandCheck = 0, lastSonicSensorRead = 0;
int* measuredDistances;
char measuredDistancesUART[1024];  // Buffer for large messages
bool changedStep = false;
int offset, measurementAttempts = 0, newScanPrecision = 0;

void setup() {
  Serial.begin(9600);       
  SerialUART.begin(19200);  
  servoM.begin();           
  Butt.init();              
  checkTimePnStep(step, timeP);  // Check and initialize step and timeP
  measuredDistances = new int[arrayLength];
}

void loop() {
  EEPROM.get(SERVO_STEP_ADR, step);
  EEPROM.get(SERVO_TIME_ADR, timeP);
  arrayLength = 360 / step;  // Calculate the number of measurements
  unsigned long currentMillis = millis();

  // Check for new commands every 500ms
  if (hasTimePassed(lastCommandCheck, currentMillis, 500)) {
    lastCommandCheck = currentMillis;

    // Process incoming command
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();  // Remove extra spaces
      processCommands(command, changedStep);
      memset(measuredDistancesUART, 0, sizeof(measuredDistancesUART));
      servoM.setDefault();
    }

    // Reallocate memory if array length has changed
    if (hasArrayLengthChanged(changedStep)) {
      delete[] measuredDistances;
      measuredDistances = new int[arrayLength];
    }

    // Check button state and initiate new scan if pressed
    if (Butt.getPinVal()) {
      vectorIndex = 0;
      newScan = 1;
      newScanPrecision = 1;  // Scan twice for calibration
      memset(measuredDistancesUART, 0, sizeof(measuredDistancesUART));
      servoM.setDefault();
    }
  }

  // Rotate and measure every `timeP` milliseconds
  if (hasTimePassed(lastSonicSensorRead, currentMillis, timeP)) {
    servoM.rotateCycle(step, timeP, poz);
    delay(300);
    distance = HC_SR04.getDistance();

    // Retry measurement up to 3 times if invalid
    while (distance == -1 && measurementAttempts < 3) {
      distance = HC_SR04.getDistance();
      measurementAttempts++;
    }
    measurementAttempts = 0;
    measuredDistances[vectorIndex] = distance;

    // Handle end of measurement cycle
    if (vectorIndex < arrayLength - 1) {
      vectorIndex++;
    } else {
      vectorIndex = 0;

      // Send message after the second scan for calibration
      if (!newScanPrecision) {
        sendLargeMessage();
        memset(measuredDistancesUART, 0, sizeof(measuredDistancesUART));
      }

      if (newScanPrecision) {
        newScanPrecision = 0;
        memset(measuredDistancesUART, 0, sizeof(measuredDistancesUART));
      }
    }
  }
}

// Check and initialize `step` and `timeP` values from EEPROM or set default
void checkTimePnStep(int step, int timeP) {
  EEPROM.get(SERVO_STEP_ADR, step);
  EEPROM.get(SERVO_TIME_ADR, timeP);

  if (step == 255 || step == -1) {
    step = 15;
    EEPROM.put(SERVO_STEP_ADR, step);
  }

  if (timeP == 255 || timeP == -1) {
    timeP = 75;
    EEPROM.put(SERVO_TIME_ADR, timeP);
  }
}

// Check if the array length has changed
int hasArrayLengthChanged(bool changed) {
  return changed ? 1 : 0;
}

// Send measured distances in chunks
void sendLargeMessage() {
  int endIndex;
  offset = sprintf(measuredDistancesUART, "S:%d:%d=", newScan, arrayLength);

  // Format the message with measured distances
  for (int i = 0; i < arrayLength; i++) {
    offset += sprintf(measuredDistancesUART + offset, "%d", measuredDistances[i]);
    if (i < arrayLength - 1) {
      offset += sprintf(measuredDistancesUART + offset, ",");
    } else {
      offset += sprintf(measuredDistancesUART + offset, "\n");
    }
  }

  newScan = 0;
  int chunkSize = 48;  

  // Send message in chunks
  for (int i = 0; i < offset; i += chunkSize) {
    int bytesToSend = chunkSize;
    if (i + chunkSize > offset) {
      bytesToSend = offset - i;
    }
    Serial.write(measuredDistancesUART + i, bytesToSend);
    delay(10);
  }
}
