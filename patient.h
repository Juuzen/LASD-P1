#ifndef patient_h
#define patient_h
#include <stdbool.h>
#include "structure.h"

bool loginCheck(Patient ptList, char fiscalCode[], char password[]);
bool isPatientRegistered(Patient ptList, char fiscalCode[]);
bool patientRegister(Patient* ptList, char fiscalCode[], char password[]);
bool patientRequestAppointment(Appointment* appList, char fiscalCode[], timeSlot slot, char symptoms[]);
//TODO: void -> bool
void patientDeleteAppointment(Appointment* appList, char fiscalCode[]);

#endif // patient_h
