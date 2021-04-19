#ifndef database_h
#define database_h
#include "structure.h"

void savePatient(char fiscalCode[], char password[]);
void savePatientListBody (Patient ptList, FILE * patientDB);
void savePatientList(Patient ptList);
Patient loadPatientList();

void saveAppointment(Appointment app);
Appointment loadAppointmentList();

#endif // database_h
