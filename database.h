#ifndef database_h
#define database_h

#include "s_patient.h"
#include "s_employee.h"
#include "s_appointment.h"
#include "s_reservation.h"
#include "s_result.h"
#include "s_quarantine.h"



/* DB PAZIENTI */
void savePatient(char fiscalCode[], char password[]);
Patient loadPatientList();


/* DB LAVORATORI LABORATORIO */
Employee loadEmployeeList();


/* DB APPUNTAMENTI */
void saveAppointment(Appointment apNode, FILE * file);
Appointment loadAppointmentList();
void saveAppointmentListBody(Appointment appList, FILE * appointmentDB);
void saveAppointmentList(Appointment appList);
void dropAppointmentDB();


/* DB RISULTATI TEST */
TestResult loadTestResultList();
void saveTestResultListBody(Appointment appList, FILE * testResultDB, int currentDay, Quarantine *qtList);
void saveTestResultList(Reservation res, Quarantine *qtList);


/* DB QUARANTENA */
Quarantine loadQuarantineList();
void saveQuarantineListBody(Quarantine qtList, FILE * file);
void saveQuarantineList(Quarantine qtList);
#endif // database_h
