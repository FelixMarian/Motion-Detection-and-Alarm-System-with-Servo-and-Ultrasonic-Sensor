#ifndef CHECKTIMEPASSING_H
#define CHECKTIMEPASSING_H

/* Used in a header for to reuse it in multiple files
   Check if a certain amount of time in ms has passed. Used in ordet to not block the code using delays */
int hasTimePassed(unsigned long &lastRead, unsigned long currentMillis, int timeInterval){
  if (currentMillis - lastRead >= timeInterval) { 
    lastRead = currentMillis;
    return 1;
    }
  return 0;
}

#endif