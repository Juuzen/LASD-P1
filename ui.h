#ifndef ui_h
#define ui_h
#include "structure.h"

void patientAccountUi(Patient ptList, char fiscalCode[]);
void patientLoginUi(Patient ptList);
void patientRegisterUi(Patient ptList);
void patientUi();
void labUi();
void mainUi();
void wrongChoice();
int getChoice(int maxOptions);
void run();

#endif // ui_h
