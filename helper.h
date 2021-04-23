#ifndef helper_h
#define helper_h
#include <stdbool.h>
#include "const.h"

void clearScreen();
void pause(char message[]);
bool generateTestResult();
int getCurrentDay();
char *getSymptoms(FILE * file);
const char* getTimeSlot(timeSlot slot);
void wrongChoice();
int getChoice(int maxOptions);

#endif // helper_h


