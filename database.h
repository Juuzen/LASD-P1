#ifndef database_h
#define database_h
#include "structure.h"

/* GESTIONE DB PAZIENTI */
void savePatient(char fiscalCode[], char password[]);
Patient loadPatientList();


/* GESTIONE DB LAVORATORI LABORATORIO */
LabWorker loadLabWorkers();


/* GESTIONE DB APPUNTAMENTI */
void saveAppointment(Appointment app);
Appointment loadAppointmentList();
void saveAppointmentListBody(Appointment appList, FILE * appointmentDB);
void saveAppointmentList(Appointment appList);
void dropAppointmentDB();


/* GESTIONE DB RISULTATI TEST */
TestResult loadTestResults();
void saveTestResultListBody(Appointment appList, FILE * testResultDB, int currentDay);
void saveTestResultList(TestReservation testList);

#endif // database_h
