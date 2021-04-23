#ifndef structure_h
#define structure_h
#include <stdbool.h>
#include "const.h"

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
Appointment deleteAppointmentByFiscalCode(Appointment appList, char fiscalCode[]);

// GESTIONE LAVORATORI LABORATORIO




// GESTIONE TEST FISSATI
struct testReservation {
    Appointment morning;
    Appointment afternoon;
    Appointment evening;
    int currentDay;
};
typedef struct testReservation* TestReservation;

TestReservation newTestReservation();
void deleteTestReservation(TestReservation reservation);
void printTestReservation(TestReservation reservation);
int appointmentListCount(Appointment appList);
bool isTimeSlotFull(TestReservation reservation, timeSlot slot);
Appointment searchAppointmentByFiscalCode(TestReservation reservation, char fiscalCode[]);
void addReservation(TestReservation *reservation, Appointment app);

// GESTIONE ESITI TEST
struct testResult {
    char fiscalCode[FISCALCODE_SIZE];
    char response[RESPONSE_SIZE];
    int day;

    struct testResult* next;
};
typedef struct testResult* TestResult;

TestResult newTestResultList ();
TestResult newTestResultNode (char fiscalCode[], char response[], int day);
void deleteTestResultNode (TestResult result);
void deleteTestResultList (TestResult rsList);
TestResult testResultInsert (TestResult rsList, char fiscalCode[], char response[], int day);
void printTestResultList (TestResult rsList, bool chronoOrder);
void printTestResultsByDay (TestResult rsList, int day, bool chronoOrder);
void printTestResultByFiscalCode (TestResult rsList, char fiscalCode[], bool chronoOrder);




#endif // structure_h


