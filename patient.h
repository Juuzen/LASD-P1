#ifndef patient_h
#define patient_h
#include <stdbool.h>
#include "structure.h"

/* FUNZIONI AUSILIARIE */
bool loginCheck(Patient ptList, char fiscalCode[], char password[]);
bool isPatientRegistered(Patient ptList, char fiscalCode[]);
bool patientRegister(Patient* ptList, char fiscalCode[], char password[]);
bool patientRequestAppointment(Appointment* appList, char fiscalCode[], timeSlot slot, char symptoms[]);
void patientDeleteAppointment(Appointment* appList, char fiscalCode[]);


/* FUNZIONI UI */
void patientShowTestResultsUi(char fiscalCode[]);
void patientAppointmentRequestUi(Appointment appList, char fiscalCode[]);
void patientShowReservationUi(TestReservation *test, char fiscalCode[]);
void patientDeleteAppointmentUi(Appointment* appList, char fiscalCode[]);
void patientAccountUi(TestReservation *test, char fiscalCode[]);
void patientLoginUi(TestReservation *test, Patient ptList);
void patientRegisterUi(Patient ptList);
void patientMainMenuUi(TestReservation *test);

#endif // patient_h
