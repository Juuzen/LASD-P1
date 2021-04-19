#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "database.h"
#include "const.h"
#include "structure.h"

void savePatient(char fiscalCode[], char password[]) {
    FILE * patientDB;
    patientDB = fopen(PATIENT_DB, "a");
    if (patientDB != NULL) {
        fprintf(patientDB, "%s\t%s\n", fiscalCode, password);
    }
    else {
        printf ("SAVE: The DB could not be opened.\n");
    }
    fclose(patientDB);
}

void savePatientListBody (Patient ptList, FILE * patientDB) {
    if (patientDB != NULL) {
        if (ptList != NULL) {
            fprintf(patientDB, "%s\t%s\n", ptList->fiscalCode, ptList->password);
            savePatientListBody(ptList->next, patientDB);
        }
        // lista caricata con successo
    }
    // file non più aperto - ERRORE
}

void savePatientList(Patient ptList) {
    FILE * patientDB;
    patientDB = fopen(PATIENT_DB, "w");
    if (patientDB != NULL) {
        savePatientListBody(ptList, patientDB);
    }
    else {
        printf ("SAVE: The DB could not be opened.\n");
        // TODO: Handle file opening error
    }
    fclose(patientDB);
}

Patient loadPatientList() {
    Patient ptList = newPatientList();
    FILE * patient_db = fopen(PATIENT_DB, "r");
    if (patient_db != NULL) {
        char fiscalCode[FISCALCODE_SIZE];
        char password[PASSWORD_SIZE];
        while ((fscanf(patient_db, "%s %s", fiscalCode, password)) != EOF) {

            ptList = patientInsert(ptList, fiscalCode, password);
        }
    } else {
        printf("LOAD: The DB could not be opened.\n");
        //TODO: Handle file opening error
    }
    fclose(patient_db);
    return ptList;
}

// GESTIONE DB APPUNTAMENTI

void saveAppointment(Appointment app) {
    FILE * appointmentDB;
    appointmentDB = fopen(APPOINTMENT_DB, "a");
    if (appointmentDB != NULL) {
        if (fprintf(appointmentDB, "%s\t%d\t%s\n", app->fiscalCode, app->slot, app->synthoms) < 0) {
            // TODO: Handle error in writing
        }
    }
    else {
        printf ("SAVE: The DB could not be opened.\n");
    }
    fclose(appointmentDB);
}

Appointment loadAppointmentList() {
    Appointment appList = newAppointmentList();
    FILE * appointmentDB = fopen(APPOINTMENT_DB, "r");
    if (appointmentDB != NULL) {
        char fiscalCode[FISCALCODE_SIZE];
        char scanSynthoms[SYNTHOMS_SIZE + 1];
        int scanSlot;
        //FIXME: Aggiustare la lettura dei sintomi
        while ((fscanf(appointmentDB, "%s\t%d\t", fiscalCode, &scanSlot) != EOF) || fgets(scanSynthoms, SYNTHOMS_SIZE + 1, appointmentDB) != NULL) {
            timeSlot slot = (timeSlot) scanSlot;
            char synthoms[SYNTHOMS_SIZE];
            (strcmp(scanSynthoms, "(null)") == 0) ? memset(synthoms, '\0', sizeof(synthoms)) : strncpy(synthoms, scanSynthoms, SYNTHOMS_SIZE);
            appList = appointmentInsert(appList, fiscalCode, slot, synthoms);
        }
    }
    else {
        //TODO: Handle file opening error
    }
    fclose(appointmentDB);
    return appList;
}
