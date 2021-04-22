#ifndef database_h
#define database_h
#include "structure.h"

void savePatient(char fiscalCode[], char password[]);
void savePatientListBody (Patient ptList, FILE * patientDB);
void savePatientList(Patient ptList);
Patient loadPatientList();

// GESTIONE DB LAVORATORI LABORATORIO
LabWorker loadLabWorkers();

void saveAppointment(Appointment app);
char *getSymptoms(FILE * file);
Appointment loadAppointmentList();
void saveAppointmentListBody(Appointment appList, FILE * appointmentDB);
void saveAppointmentList(Appointment appList);
void dropAppointmentDB();

// GESTIONE RISULTATI TEST
int getCurrentDay();
TestResult loadTestResults();
void saveTestResultListBody(Appointment appList, FILE * testResultDB, int currentDay);
void saveTestResultList(TestReservation testList);

#endif // database_h
