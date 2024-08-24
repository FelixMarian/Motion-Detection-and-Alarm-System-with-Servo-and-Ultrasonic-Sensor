#ifndef PROCESSCOMMANDS_H
#define PROCESSCOMMANDS_H

#include "Addresses_n_Macros.h"
#include <EEPROM.h>

// Function that processes the available commands
void processCommands(String command, bool &stepChange) {
  int firstSpaceIndex;

  // Check if command is to set servo values
  if (command.startsWith("SET_SERVO ")) {
    firstSpaceIndex = command.indexOf(' ', 10);
    if (firstSpaceIndex != -1) {
      int step = command.substring(10, firstSpaceIndex).toInt(); // Get step value from command
      int timeP = command.substring(firstSpaceIndex + 1).toInt(); // Get time value from command

      // Validate and save step and time values if they are within valid ranges
      if(step <= 180 && step > 0 && timeP > 0){
        EEPROM.put(SERVO_STEP_ADR, step);
        EEPROM.put(SERVO_TIME_ADR, timeP);
        stepChange = true; // Indicate that step values have changed
      }
    }
  }

  // Reset servo values to default
  if (command == "SET_SERVO_DEFAULT") {
    EEPROM.put(SERVO_STEP_ADR, 15); // Default step value
    EEPROM.put(SERVO_TIME_ADR, 75); // Default time value
    stepChange = true; // Indicate that step values have changed
  }

  // Stop the servo from rotating
  if (command == "STOP_SERVO") {
    EEPROM.put(SERVO_STEP_ADR, 0); // Set step to 0 to stop the servo
    EEPROM.put(SERVO_TIME_ADR, 500); // Set a default time value
    stepChange = true; // Indicate that step values have changed
  }
}

#endif
