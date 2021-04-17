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

void printPatientList(Patient ptList) {
    if (ptList != NULL) {
        printf("%s | %s\n", ptList->fiscalCode, ptList->password);
        printPatientList(ptList->next);
    }
}

// GESTIONE APPUNTAMENTI

Appointment newAppointmentNode(char fiscalCode[], char synthoms[], timeSlot slot) {
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