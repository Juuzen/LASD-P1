#ifndef database_h
#define database_h
#include "structure.h"

void savePatient(char fiscalCode[], char password[]);
void savePatientListBody (Patient ptList, FILE * patientDB);
void savePatientList(Patient ptList);
Patient loadPatientList();

void saveAppointment(Appointment app);
char *getSymptoms(FILE * file);
Appointment loadAppointmentList();
void saveAppointmentListBody(Appointment appList, FILE * appointmentDB);
void saveAppointmentList(Appointment appList);

#endif // database_h
