#include "ProcessMessage.h"
#include "CompareArrays.h"
#include <LiquidCrystal.h>
#include "Buzzer.h"
#include "RGBmodule.h"

char receivedMessage[1024];  // Buffer for incoming messages
int receivedIndex = 0;
int processedDistances[360];
int firstScan, arrayLength, lastIndex;
String receivedMessageString;
int arrayNo = 1;  // Start with array 1 by default
int firstScanRecDist[360], firstRecDist[360], secondRecDist[360];
int rs = 22, e = 23, db4 = 24, db5 = 25, db6 = 26, db7 = 27, pinStopAlarmButt = 48;
bool scannedRef = false, isAlarmOn = false;  // Flags for reference scan and alarm status

Buzzer buzzer(47);
LiquidCrystal LCD(rs, e, db4, db5, db6, db7);
RGBmodule RGB(35, 37, 39);

void setup() {
  Serial.begin(9600);    // Initialize USB serial
  Serial1.begin(19200);  // Initialize UART serial
  LCD.begin(16, 2);      // Set up LCD with 16 columns and 2 rows
  pinMode(pinStopAlarmButt, INPUT);  // Set button pin as input
}

void loop() {
  // Read data from UART if available
  while (Serial1.available()) {
    char c = Serial1.read();  // Read a character
    if (receivedIndex < sizeof(receivedMessage) - 1) {
      receivedMessage[receivedIndex++] = c;  // Store character in buffer
    }
    // End of message indicated by '\n'
    if (c == '\n') {
      receivedMessage[receivedIndex] = '\0';            // Null-terminate the string
      receivedMessageString = String(receivedMessage);  // Convert to String for processing
      memset(receivedMessage, 0, 1024);                 // Clear buffer
      receivedIndex = 0;                                // Reset index for next message

      processMessage(firstScan, arrayLength, processedDistances, receivedMessageString);  // Process the message

      if (firstScan) {
        // Store processed distances in reference array on first scan
        for (int index = 0; index < arrayLength; index++)
          firstScanRecDist[index] = processedDistances[index];
        firstScan = 0;
        scannedRef = true;
        memset(firstRecDist, 0, 360);
        memset(secondRecDist, 0, 360);
      } else {
        // Alternate between storing in firstRecDist and secondRecDist
        if (arrayNo == 1) {
          for (int index = 0; index < arrayLength; index++)
            firstRecDist[index] = processedDistances[index];
          arrayNo = 2;
        } else if (arrayNo == 2) {
          for (int index = 0; index < arrayLength; index++)
            secondRecDist[index] = processedDistances[index];
          arrayNo = 1;

          if (scannedRef) {
            // Compare the arrays and activate alarm if similarity is below threshold
            int comparisonResult = compareArrays(firstRecDist, secondRecDist, firstScanRecDist, arrayLength);
            if (comparisonResult < 60)
              isAlarmOn = true;
          }
        }
      }
    }
  }

  // Display status messages based on reference scan and alarm state
  if (scannedRef) {
    RGB.G_on();
    if (arrayNo == 2) {
      LCD.setCursor(0, 0);
      LCD.print("Scanning 2/2...");
      LCD.setCursor(0, 1);
      LCD.print("Please wait     ");
    } else if (arrayNo == 1) {
      LCD.setCursor(0, 0);
      LCD.print("Scanning 1/2...");
      LCD.setCursor(0, 1);
      LCD.print("Please wait     ");
    }
  } else {
    if (!isAlarmOn)
      RGB.Y_on();
    LCD.setCursor(0, 0);
    LCD.print("Start to scan");
    LCD.setCursor(0, 1);
    LCD.print("Press the button");
  }

  // Manage alarm state
  while (isAlarmOn) {
    buzzer.AlarmOn();
    RGB.R_on();
    LCD.setCursor(0, 0);
    LCD.print("Stop the alarm  ");
    LCD.setCursor(0, 1);
    LCD.print("Press the button");
    if (digitalRead(pinStopAlarmButt) == HIGH)
      isAlarmOn = false;
  }
  buzzer.AlarmOff();
}
