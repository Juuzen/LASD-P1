#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structure.h"
#include "database.h"
#include "helper.h"

// GESTIONE TEST RESERVATIONS
TestReservation newTestReservation() {
    TestReservation reservation = (TestReservation) calloc(1, sizeof(struct testReservation));
    reservation->morning = NULL;
    reservation->afternoon = NULL;
    reservation->evening = NULL;
    reservation->currentDay = getCurrentDay();
    return reservation;
}
void deleteTestReservation(TestReservation reservation) {
    if (reservation != NULL) {
    appointmentFreeList(reservation->morning);
    appointmentFreeList(reservation->afternoon);
    appointmentFreeList(reservation->evening);
    reservation->morning = NULL;
    reservation->afternoon = NULL;
    reservation->evening = NULL;
    reservation->currentDay = 0;
    free(reservation);
    }
}
void printTestReservation(TestReservation reservation) {
    if (reservation != NULL) {
        printf("DAY %d:\n", reservation->currentDay);
        printf("Morning tests:\n");
        if (reservation->morning == NULL) printf("No reservations.\n");
        else appointmentPrintList(reservation->morning);

        printf("\nAfternoon tests:\n");
        if (reservation->afternoon == NULL) printf("No reservations.\n");
        else appointmentPrintList(reservation->afternoon);

        printf("\nEvening tests:\n");
        if (reservation->evening == NULL) printf("No reservations.\n\n");
        else appointmentPrintList(reservation->evening);
    }
}
int appointmentListCount(Appointment appList) {
    if (appList == NULL) return 0;
    else return 1 + appointmentListCount(appList->next);
}
bool isTimeSlotFull(TestReservation reservation, timeSlot slot) {
    bool response = true;
    if (reservation != NULL) {
        switch (slot) {
            case MORNING:
                if (appointmentListCount(reservation->morning) < 2) {
                    response = false;
                }
                break;
            case AFTERNOON:
                if (appointmentListCount(reservation->afternoon) < 2) {
                    response = false;
                }
                break;
            case EVENING:
                if (appointmentListCount(reservation->evening) < 2) {
                    response = false;
                }
                break;
            default:
                break;
        }
    }

    return response;
}
Appointment searchAppointmentByFiscalCode(TestReservation reservation, char fiscalCode[]) {
    Appointment app = NULL;
    if (reservation != NULL) {
        app = appointmentFindByFiscalCode(reservation->morning, fiscalCode);
        if (app == NULL) {
            app = appointmentFindByFiscalCode(reservation->afternoon, fiscalCode);
            if (app == NULL) {
                app = appointmentFindByFiscalCode(reservation->evening, fiscalCode);
            }
        }
    }
    return app;
}
void addReservation(TestReservation *reservation, Appointment app) {
    if ((*reservation) != NULL) {
        if (app != NULL) {
            switch (app->slot) {
                case MORNING:
                    (*reservation)->morning = appointmentAppendToList((*reservation)->morning, app);
                    break;
                case AFTERNOON:
                    (*reservation)->afternoon = appointmentAppendToList((*reservation)->afternoon, app);
                    break;
                case EVENING:
                    (*reservation)->evening = appointmentAppendToList((*reservation)->evening, app);
                    break;
                default:
                    break;
            }
        }
    }
}

// GESTIONE ESITI TEST
TestResult newTestResultList() {
    TestResult list = NULL;
    return list;
}
TestResult newTestResultNode(char fiscalCode[], char response[], int day) {
    TestResult result = (TestResult) calloc(1, sizeof(struct testResult));
    if (result != NULL) {
        strcpy(result->fiscalCode, fiscalCode);
        strcpy(result->response, response);
        result->day = day;
        result->next = NULL;
    }

    return result;
}
void deleteTestResultNode(TestResult result) {
    if (result != NULL) {
        memset(result->fiscalCode, '\0', sizeof(result->fiscalCode));
        memset(result->response, '\0', sizeof(result->response));
        result->day = 0;

        free(result);
    }
}
void deleteTestResultList(TestResult rsList) {
    if (rsList != NULL) {
        deleteTestResultList(rsList->next);
        deleteTestResultNode(rsList);
    }
}
TestResult testResultInsert(TestResult rsList, char fiscalCode[], char response[], int day) {
    if (rsList == NULL) return newTestResultNode(fiscalCode, response, day);
    else {
        rsList->next = testResultInsert(rsList->next, fiscalCode, response, day);
        return rsList;
    }
}
void printTestResultList(TestResult rsList, bool chronoOrder) {
    if (rsList != NULL) {
        if (!chronoOrder) {
            // In questa maniera viene stampato in maniera anti-cronologica (più recente -> meno recente)
            printTestResultList(rsList->next, chronoOrder);
        }

        printf("DAY: %d - %s: %s\n", rsList->day, rsList->fiscalCode, rsList->response);

        if (chronoOrder) {
            // In questa maniera viene stampato in maniera cronologica (meno recente -> più recente)
            printTestResultList(rsList->next, chronoOrder);
        }

    }
}
void printTestResultsByDay (TestResult rsList, int day, bool chronoOrder) {
    if (rsList != NULL) {
        if (!chronoOrder) {
            // In questa maniera viene stampato in maniera anti-cronologica (più recente -> meno recente)
            printTestResultsByDay(rsList->next, day, chronoOrder);
        }

        if (rsList->day == day) {
            printf("Patient: %s - %s\n", rsList->fiscalCode, rsList->response);
        }

        if (chronoOrder) {
            // In questa maniera viene stampato in maniera cronologica (meno recente -> più recente)
            printTestResultsByDay(rsList->next, day, chronoOrder);
        }



    }
}
void printTestResultByFiscalCode (TestResult rsList, char fiscalCode[], bool chronoOrder) {
    if (rsList != NULL) {
        if (!chronoOrder) {
            // In questa maniera viene stampato in maniera anti-cronologica (più recente -> meno recente)
            printTestResultByFiscalCode(rsList->next, fiscalCode, chronoOrder);
        }

        if ((strcmp(rsList->fiscalCode, fiscalCode)) == 0) {
            printf("DAY %d - %s\n", rsList->day, rsList->response);
        }

        if (chronoOrder) {
            // In questa maniera viene stampato in maniera cronologica (meno recente -> più recente)
            printTestResultByFiscalCode(rsList->next, fiscalCode, chronoOrder);
        }

    }
}
