#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structure.h"
#include "database.h"
#include "helper.h"



// GESTIONE APPUNTAMENTI

const char* getTimeSlot(timeSlot slot) {
    switch (slot) {
        case MORNING:
            return "MORNING";
        case AFTERNOON:
            return "AFTERNOON";
        case EVENING:
            return "EVENING";
        default:
            return "";
    }
}
Appointment newAppointmentList() {
    Appointment app = NULL;
    return app;
}
Appointment newAppointmentNode(char fiscalCode[], timeSlot slot, char symptoms[]) {
    Appointment app = (Appointment) calloc(1, sizeof(struct appointment));

    if (app != NULL) {
        strcpy(app->fiscalCode, fiscalCode);
        if (symptoms != NULL) {
            app->symptoms = (char *) calloc(1, SYMPTOMS_SIZE * sizeof(char));
            strcpy(app->symptoms, symptoms);
        }
        app->slot = slot;
    }
    return app;
}
Appointment cloneAppointment(Appointment app) {
    if (app == NULL) return NULL;
    else return newAppointmentNode(app->fiscalCode, app->slot, app->symptoms);
}
void deleteAppointmentNode (Appointment app) {
    if (app != NULL) {
        memset(app->fiscalCode, '\0', sizeof(app->fiscalCode));
        if (app->symptoms != NULL) {
            free(app->symptoms);
            app->symptoms = NULL;
        }
        app->slot = 0;
        free(app);
    }
}
void deleteAppointmentList (Appointment appList) {
    if (appList != NULL) {
        deleteAppointmentList(appList->next);
        deleteAppointmentNode(appList);
    }
}
Appointment appointmentAppend(Appointment first, Appointment append) {
    if (first == NULL) {
        return append;
    }

    else {
        first->next = appointmentAppend(first->next, append);
        return first;
    }
}
Appointment appointmentInsert(Appointment appList, char fiscalCode[], timeSlot slot, char symptoms[]) {
    if (appList == NULL) {
        return newAppointmentNode(fiscalCode, slot, symptoms);
    }

    else {
        appList->next = appointmentInsert(appList->next, fiscalCode, slot, symptoms);
        return appList;
    }
}
void printAppointmentNode(Appointment app) {
    if (app != NULL) {
        printf("Fiscal Code: %s\n", app->fiscalCode);
        printf("Time slot: %s\n", getTimeSlot(app->slot));
        printf("Symptoms: ");
        printf((app->symptoms == NULL) ? "///" : app->symptoms);
        printf("\n\n");
    }
}
void printAppointmentList(Appointment appList) {
    if (appList != NULL) {
        printAppointmentNode(appList);
        printAppointmentList(appList->next);
    }
}
Appointment findAppointmentByFiscalCode(Appointment appList, char fiscalCode[]) {
    if (appList == NULL) return NULL;
    else {
        if (strcmp(appList->fiscalCode, fiscalCode) == 0) return appList;
        else return findAppointmentByFiscalCode(appList->next, fiscalCode);
    }
}
Appointment deleteAppointmentByFiscalCode(Appointment appList, char fiscalCode[]) {
    if (appList == NULL) return appList;
    else {
        if (strcmp(appList->fiscalCode, fiscalCode) == 0) {
            Appointment tmp = appList->next;
            deleteAppointmentNode(appList);
            return tmp;
        }
        else {
            appList->next = deleteAppointmentByFiscalCode(appList->next, fiscalCode);
            return appList;
        }
    }
}

// GESTIONE LAVORATORI


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
    deleteAppointmentList(reservation->morning);
    deleteAppointmentList(reservation->afternoon);
    deleteAppointmentList(reservation->evening);
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
        else printAppointmentList(reservation->morning);

        printf("\nAfternoon tests:\n");
        if (reservation->afternoon == NULL) printf("No reservations.\n");
        else printAppointmentList(reservation->afternoon);

        printf("\nEvening tests:\n");
        if (reservation->evening == NULL) printf("No reservations.\n\n");
        else printAppointmentList(reservation->evening);
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
        app = findAppointmentByFiscalCode(reservation->morning, fiscalCode);
        if (app == NULL) {
            app = findAppointmentByFiscalCode(reservation->afternoon, fiscalCode);
            if (app == NULL) {
                app = findAppointmentByFiscalCode(reservation->evening, fiscalCode);
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
                    (*reservation)->morning = appointmentAppend((*reservation)->morning, app);
                    break;
                case AFTERNOON:
                    (*reservation)->afternoon = appointmentAppend((*reservation)->afternoon, app);
                    break;
                case EVENING:
                    (*reservation)->evening = appointmentAppend((*reservation)->evening, app);
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
            // In questa maniera viene stampato in maniera anti-cronologica (pi� recente -> meno recente)
            printTestResultList(rsList->next, chronoOrder);
        }

        printf("DAY: %d - %s: %s\n", rsList->day, rsList->fiscalCode, rsList->response);

        if (chronoOrder) {
            // In questa maniera viene stampato in maniera cronologica (meno recente -> pi� recente)
            printTestResultList(rsList->next, chronoOrder);
        }

    }
}
void printTestResultsByDay (TestResult rsList, int day, bool chronoOrder) {
    if (rsList != NULL) {
        if (!chronoOrder) {
            // In questa maniera viene stampato in maniera anti-cronologica (pi� recente -> meno recente)
            printTestResultsByDay(rsList->next, day, chronoOrder);
        }

        if (rsList->day == day) {
            printf("Patient: %s - %s\n", rsList->fiscalCode, rsList->response);
        }

        if (chronoOrder) {
            // In questa maniera viene stampato in maniera cronologica (meno recente -> pi� recente)
            printTestResultsByDay(rsList->next, day, chronoOrder);
        }



    }
}
void printTestResultByFiscalCode (TestResult rsList, char fiscalCode[], bool chronoOrder) {
    if (rsList != NULL) {
        if (!chronoOrder) {
            // In questa maniera viene stampato in maniera anti-cronologica (pi� recente -> meno recente)
            printTestResultByFiscalCode(rsList->next, fiscalCode, chronoOrder);
        }

        if ((strcmp(rsList->fiscalCode, fiscalCode)) == 0) {
            printf("DAY %d - %s\n", rsList->day, rsList->response);
        }

        if (chronoOrder) {
            // In questa maniera viene stampato in maniera cronologica (meno recente -> pi� recente)
            printTestResultByFiscalCode(rsList->next, fiscalCode, chronoOrder);
        }

    }
}
