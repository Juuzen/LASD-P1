#ifndef ui_h
#define ui_h
#include "structure.h"

/* UI PAZIENTI */
void patientShowTestResultsUi(char fiscalCode[]);
void patientAppointmentRequestUi(Appointment appList, char fiscalCode[]);
void patientShowAppointmentUi(Appointment appList, char fiscalCode[]);
void patientDeleteAppointmentUi(Appointment* appList, char fiscalCode[]);
void patientAccountUi(char fiscalCode[]);
void patientLoginUi(Patient ptList);
void patientRegisterUi(Patient ptList);
void patientUi();

/* UI LABORATORIO */
void labShowTestHistoryUi(int currentDay);
void labLoginUi(TestReservation *test);
void labUi(TestReservation *test);

/* FUNZIONI AUSILIARIE */
void wrongChoice();
int getChoice(int maxOptions);

/* AVVIO PROGRAMMA */
void mainUi(TestReservation *test);
void run();

#endif // ui_h
