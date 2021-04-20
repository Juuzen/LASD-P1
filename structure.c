#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structure.h"

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

// GESTIONE STRUTTURA TEST
TestingDay newTestingDay() {
    TestingDay test = (TestingDay) calloc(1, sizeof(struct testingDay));
    return test;
}

void deleteTestingDay(TestingDay test) {
    if (test != NULL) {
    deleteAppointmentList(test->morning);
    deleteAppointmentList(test->afternoon);
    deleteAppointmentList(test->evening);
    test->morning = NULL;
    test->afternoon = NULL;
    test->evening = NULL;

    free(test);
    }
}

void printTestingDay(TestingDay test) {
    if (test != NULL) {
        printf("Morning tests:\n");
        printAppointmentList(test->morning);

        printf("\nAfternoon tests:\n");
        printAppointmentList(test->afternoon);

        printf("\nEvening tests:\n");
        printAppointmentList(test->evening);
    }
}

int appointmentListCount(Appointment appList) {
    if (appList == NULL) return 0;
    else return 1 + appointmentListCount(appList->next);
}

bool isTimeSlotFull(TestingDay test, timeSlot slot) {
    bool response = true;
    if (test != NULL) {
        switch (slot) {
            case MORNING:
                if (appointmentListCount(test->morning) < 2) {
                    response = false;
                }
                break;
            case AFTERNOON:
                if (appointmentListCount(test->afternoon) < 2) {
                    response = false;
                }
                break;
            case EVENING:
                if (appointmentListCount(test->evening) < 2) {
                    response = false;
                }
                break;
            default:
                break;
        }
    }

    return response;
}
