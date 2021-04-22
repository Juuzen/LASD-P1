#ifndef ui_h
#define ui_h
#include "structure.h"

/* UI PAZIENTI */
void patientShowTestResultsUi(char fiscalCode[]);
void patientAppointmentRequestUi(Appointment appList, char fiscalCode[]);
void patientShowReservationUi(TestReservation *test, char fiscalCode[]);
void patientDeleteAppointmentUi(Appointment* appList, char fiscalCode[]);
void patientAccountUi(TestReservation *test, char fiscalCode[]);
void patientLoginUi(TestReservation *test, Patient ptList);
void patientRegisterUi(Patient ptList);
void patientUi(TestReservation *test);

/* UI LABORATORIO */
void labShowTestHistoryUi(int currentDay);
void labManageAppointmentRequestsUi(TestReservation *test);
void labShowReservationUi(TestReservation test);
void labAddReservationUi(TestReservation *test);
void labRemoveReservationUi(TestReservation *test);
void labHandleReservationUi(TestReservation *test);
void labLoginUi(TestReservation *test);
void labUi(TestReservation *test);

/* FUNZIONI AUSILIARIE */
void wrongChoice();
int getChoice(int maxOptions);

/* AVVIO PROGRAMMA */
void mainUi(TestReservation *test);
void run();

#endif // ui_h
