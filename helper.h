#ifndef helper_h
#define helper_h
#include <stdbool.h>
#include "const.h"

void clearScreen();
char* maskedInput();
void printMessage(char message[]);
bool generateTestResult();
int getEmployeeId(char message[]);
int getCurrentDay();
char *getSymptoms(FILE * file);
const char* getTimeSlot(timeSlot slot);
void wrongChoice();
int getChoice(int maxOptions);

#endif // helper_h


