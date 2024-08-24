#ifndef PROCESSMESSAGE_H
#define PROCESSMESSAGE_H

void processMessage(int &firstScan, int &arraySize, int measuredDistances[], String receivedMessage) {
  int messageLength = receivedMessage.length();  // Length of the received message
  char receivedMessageChar[messageLength + 1];   // Convert message to char array for strtok
  int measuredDistancesIndex = 0;                // Index for measuredDistances array

  // Find position of '=' in the message
  int equalsPos = receivedMessage.indexOf("=", sizeof(receivedMessage));

  // Extract the part of the message after '='
  String receivedMessageJustValues = receivedMessage.substring(equalsPos + 1);

  // Extract firstScan value (position 2-3)
  firstScan = receivedMessage.substring(2, 3).toInt();

  // Extract arraySize (from position 4 to '=')
  arraySize = receivedMessage.substring(4, equalsPos).toInt();

  // Convert values part of the message to char array
  receivedMessageJustValues.toCharArray(receivedMessageChar, messageLength + 1);

  // Split the char array by commas
  char *value = strtok(receivedMessageChar, ",");
  while (value != NULL) {
    // Convert each token to an integer and store in measuredDistances
    measuredDistances[measuredDistancesIndex++] = atoi(value);
    value = strtok(NULL, ",");  // Get next token
    if (value != NULL) {
      measuredDistances[measuredDistancesIndex] = "\0";  // Null-terminate string (unnecessary but kept)
    }
  }
}

#endif
