#ifndef structure_h
#define structure_h
#include "const.h"

// GESTIONE LISTA PAZIENTI

typedef struct patient {
    char fiscalCode[FISCALCODE_SIZE];
    char password[PASSWORD_SIZE];
    struct patient* next;
} patient;
typedef patient* Patient;

Patient newPatientList();
Patient newPatientNode(char fiscalCode[], char password[]);
Patient patientInsert(Patient ptList, char fiscalCode[], char password[]);
void printPatientList(Patient ptList); //debug

// GESTIONE CODA APPUNTAMENTI
typedef enum timeSlot { DAY, AFTERNOON, EVENING } timeSlot;

struct appointment {
    char fiscalCode[FISCALCODE_SIZE];
    char *synthoms;
    timeSlot slot;
};
typedef struct appointment* Appointment;

typedef struct testingDay {
    Appointment morning[2];
    Appointment afternoon[2];
    Appointment evening[2];
} testingDay;

Appointment newAppointmentNode(char fiscalCode[], char synthoms[], timeSlot slot);
#endif // structure_h
