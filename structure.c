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

Appointment newAppointmentNode(char fiscalCode[], timeSlot slot, char synthoms[]) {
    Appointment app = (Appointment) calloc(1, sizeof(struct appointment));

    if (app != NULL) {
        strcpy(app->fiscalCode, fiscalCode);
        if (synthoms != NULL) {
            app->synthoms = (char *) calloc(1, SYNTHOMS_SIZE * sizeof(char));
            strcpy(app->synthoms, synthoms);
        }
        app->slot = slot;
    }
    return app;
}

void deleteAppointmentNode (Appointment app) {
    if (app != NULL) {
        memset(app->fiscalCode, '\0', sizeof(app->fiscalCode));
        if (app->synthoms != NULL) {
            free(app->synthoms);
            app->synthoms = NULL;
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

Appointment appointmentInsert(Appointment appList, char fiscalCode[], timeSlot slot, char synthoms[]) {
    if (appList == NULL) {
        return newAppointmentNode(fiscalCode, slot, synthoms);
    }

    else {
        appList->next = appointmentInsert(appList->next, fiscalCode, slot, synthoms);
        return appList;
    }
}

void printAppointmentNode(Appointment app) {
    printf("Fiscal Code: %s\n", app->fiscalCode);
    printf("Time slot: %s\n", getTimeSlot(app->slot));
    printf("Synthoms: ");
    printf((app->synthoms == NULL) ? "None" : app->synthoms);
    printf("\n\n");
}

void printAppointmentList(Appointment appList) {
    if (appList != NULL) {
        printAppointmentNode(appList);
        printAppointmentList(appList->next);
    }
}
