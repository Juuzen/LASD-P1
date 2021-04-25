#ifndef patient_h
#define patient_h
#include <stdbool.h>

#include "s_patient.h"
#include "s_quarantine.h"

/* FUNZIONI AUSILIARIE */
bool loginCheck(Patient ptList, char fiscalCode[], char password[]);
bool isPatientRegistered(Patient ptList, char fiscalCode[]);
bool patientRegister(Patient* ptList, char fiscalCode[], char password[]);
bool patientRequestAppointment(Appointment* appList, char fiscalCode[], timeSlot slot, char symptoms[]);
void patientDeleteAppointment(Appointment* appList, char fiscalCode[]);


/* FUNZIONI UI */
void patientShowTestResultsUi(char fiscalCode[], int currentDay);
void patientAppointmentRequestUi(Appointment appList, char fiscalCode[]);
void patientShowReservationUi(Reservation *res, char fiscalCode[]);
void patientDeleteAppointmentUi(Appointment* appList, char fiscalCode[]);
void patientAccountUi(Reservation *res, char fiscalCode[], bool quarantined);
void patientLoginUi(Reservation *res, Patient ptList, Quarantine qtList);
void patientRegisterUi(Patient *ptList);
void patientMainMenuUi(Reservation *res, Quarantine qtList);

#endif // patient_h
