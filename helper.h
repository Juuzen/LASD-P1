#ifndef helper_h
#define helper_h
#include <stdbool.h>

void clearScreen();
void pause(char message[]);
bool generateTestResult();
int getCurrentDay();
char *getSymptoms(FILE * file);
void wrongChoice();
int getChoice(int maxOptions);

#endif // helper_h


