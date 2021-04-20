#ifndef ui_h
#define ui_h
#include "structure.h"

/* UI PAZIENTI */
void patientAppointmentRequestUi(Appointment appList, char fiscalCode[]);
void patientShowAppointmentUi(Appointment appList, char fiscalCode[]);
void patientDeleteAppointmentUi(Appointment* appList, char fiscalCode[]);
void patientAccountUi(char fiscalCode[]);
void patientLoginUi(Patient ptList);
void patientRegisterUi(Patient ptList);
void patientUi();

/* UI LABORATORIO */
void labUi();

/* FUNZIONI AUSILIARIE */
void wrongChoice();
int getChoice(int maxOptions);

/* AVVIO PROGRAMMA */
void mainUi();
void run();

#endif // ui_h
