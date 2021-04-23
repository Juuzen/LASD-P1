#ifndef structure_h
#define structure_h
#include <stdbool.h>
#include "s_appointment.h"
#include "const.h"


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


