#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structure.h"
#include "database.h"

Patient newPatientList() {
    Patient pt = NULL;
    return pt;
}
Patient newPatientNode(char fiscalCode[], char password[]) {
    Patient pt = (Patient) calloc(1, sizeof(patient));
    pt->next = NULL;
    strncpy(pt->fiscalCode, fiscalCode, FISCALCODE_SIZE + 1);
    strncpy(pt->password, password, PASSWORD_SIZE + 1);
    // TODO: Assicurarsi che alla fine delle stringhe vi sia la terminazione
    return pt;
}
Patient patientInsert(Patient ptList, char fiscalCode[], char password[]) {
    if (ptList == NULL) {
        return newPatientNode(fiscalCode, password);
    }
    else {
        ptList->next = patientInsert(ptList->next, fiscalCode, password);
        return ptList;
    }
}
void deletePatientList(Patient ptList) {
    if (ptList != NULL) {
        deletePatientList(ptList->next);
        memset(ptList->fiscalCode, '\0', sizeof(ptList->fiscalCode));
        memset(ptList->password, '\0', sizeof(ptList->password));
        ptList->next = NULL;
        free(ptList);
    }
}
void printPatientList(Patient ptList) {
    if (ptList != NULL) {
        printf("%s | %s\n", ptList->fiscalCode, ptList->password);
        printPatientList(ptList->next);
    }
}

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
        printf("symptoms: ");
        printf((app->symptoms == NULL) ? "None" : app->symptoms);
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

LabWorker newLabWorkerList() {
    LabWorker list = NULL;
    return list;
}
LabWorker newLabWorkerNode(int workerId, char password[]) {
    LabWorker worker = (LabWorker) calloc(1, sizeof(struct labWorker));
    if (worker != NULL) {
        worker->id = workerId;
        strcpy(worker->password, password);
        worker->next = NULL;
    }

    return worker;
}
void deleteLabWorkerNode(LabWorker worker) {
    if (worker != NULL) {
        worker->id = 0;
        memset(worker->password, '\0', sizeof(worker->password));
        worker->next = NULL;

        free(worker);
    }
}
void deleteLabWorkerList(LabWorker wkList) {
    if (wkList != NULL) {
        deleteLabWorkerList(wkList->next);
        deleteLabWorkerNode(wkList);
    }
}
LabWorker labWorkerInsert(LabWorker wkList, int workerId, char password[]) {
    if (wkList == NULL) return newLabWorkerNode(workerId, password);
    else {
        wkList->next = labWorkerInsert(wkList->next, workerId, password);
        return wkList;
    }
}
void printLabWorkers(LabWorker wkList) {
    if (wkList != NULL) {
        printf("%d - %s\n", wkList->id, wkList->password);
        printLabWorkers(wkList->next);
    }
}

// GESTIONE STRUTTURA TEST
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
void printTestingDay(TestReservation reservation) {
    if (reservation != NULL) {
        printf("DAY %d:\n", reservation->currentDay);
        printf("Morning tests:\n");
        printAppointmentList(reservation->morning);

        printf("\nAfternoon tests:\n");
        printAppointmentList(reservation->afternoon);

        printf("\nEvening tests:\n");
        printAppointmentList(reservation->evening);
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



