#ifndef COMPAREARRAYS_H
#define COMPAREARRAYS_H

// Check if arrElem is within ±25% of fS_elem
int isItClose(int fS_elem, int arrElem) {
  if (arrElem >= 0.75 * fS_elem && arrElem <= 1.25 * fS_elem)
    return 1;
  return 0;
}

// Compare two arrays against a reference array (firstScanArr)
int compareArrays(int firstDistancesArr[], int secondDistancesArr[], int firstScanArr[], int arrLength) {
  int avg, validValues = 0, closeEnoughValues = 0;

  for (int i = 0; i < arrLength; i++) {
    // Skip positions with invalid (-1) elements
    if (firstScanArr[i] == -1 || firstDistancesArr[i] == -1 || secondDistancesArr[i] == -1) continue;
    
    avg = (firstDistancesArr[i] + secondDistancesArr[i]) / 2;  // Calculate average of current values
    validValues++;  // Count valid comparisons
    if (isItClose(firstScanArr[i], avg))
      closeEnoughValues++;  // Count values within ±25% range
  }

  int val = (int)(((float)closeEnoughValues / validValues) * 100);  // Calculate percentage of close values

  return val;  // Return accuracy percentage
}

#endif
