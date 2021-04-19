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
void deletePatientList(Patient ptList);
void printPatientList(Patient ptList); //debug

// GESTIONE CODA APPUNTAMENTI
typedef enum timeSlot { MORNING, AFTERNOON, EVENING } timeSlot;

struct appointment {
    char fiscalCode[FISCALCODE_SIZE];
    timeSlot slot;
    char *synthoms;

    struct appointment* next;
};
typedef struct appointment* Appointment;

typedef struct testingDay {
    Appointment morning[2];
    Appointment afternoon[2];
    Appointment evening[2];
} testingDay;

const char* getTimeSlot(timeSlot slot);

Appointment newAppointmentList();
Appointment newAppointmentNode(char fiscalCode[], timeSlot slot, char synthoms[]);
void deleteAppointmentNode (Appointment app);
void deleteAppointmentList(Appointment appList);
Appointment appointmentInsert(Appointment appList, char fiscalCode[], timeSlot slot, char synthoms[]);
void printAppointmentNode(Appointment app);
void printAppointmentList(Appointment appList);
#endif // structure_h
