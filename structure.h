#ifndef structure_h
#define structure_h
#include <stdbool.h>
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
    char *symptoms;

    struct appointment* next;
};
typedef struct appointment* Appointment;

const char* getTimeSlot(timeSlot slot);
Appointment newAppointmentList();
Appointment newAppointmentNode(char fiscalCode[], timeSlot slot, char symptoms[]);
Appointment cloneAppointment(Appointment app);
void deleteAppointmentNode (Appointment app);
void deleteAppointmentList(Appointment appList);
Appointment appointmentAppend(Appointment first, Appointment append);
Appointment appointmentInsert(Appointment appList, char fiscalCode[], timeSlot slot, char symptoms[]);
void printAppointmentNode(Appointment app);
void printAppointmentList(Appointment appList);
Appointment findAppointmentByFiscalCode(Appointment appList, char fiscalCode[]);

// GESTIONE INFO TEST
struct testingDay {
    Appointment morning;
    Appointment afternoon;
    Appointment evening;
};
typedef struct testingDay* TestingDay;

TestingDay newTestingDay();
void deleteTestingDay(TestingDay test);
void printTestingDay(TestingDay test);
int appointmentListCount(Appointment appList);
bool isTimeSlotFull(TestingDay test, timeSlot slot);
#endif // structure_h

