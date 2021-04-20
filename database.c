#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "database.h"
#include "const.h"
#include "structure.h"

// GESTIONE DB PAZIENTI

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

//useless
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

//useless
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
        if (fprintf(appointmentDB, "%s\t%d\t%s\n", app->fiscalCode, app->slot, app->symptoms) < 0) {
            // TODO: Handle error in writing
        }
    }
    else {
        printf ("SAVE: The DB could not be opened.\n");
    }
    fclose(appointmentDB);
}

char * getSymptoms(FILE * file) {
    if (file != NULL) {
        int i = 0;
        char c;
        char * scan = (char *) calloc(1, SYMPTOMS_SIZE * sizeof(char));
        while (((c = fgetc(file)) != EOF) && c != '\n' && i < SYMPTOMS_SIZE) {
            scan[i] = c;
            i++;
        }

        if (feof(file)) {
            printf("FILE FINITO\n");
            return NULL;
        }
        return scan;
    }
    else return NULL;
}

Appointment loadAppointmentList() {
    Appointment appList = newAppointmentList();
    FILE * appointmentDB = fopen(APPOINTMENT_DB, "r");
    if (appointmentDB != NULL) {
        Appointment synthomaticList = newAppointmentList();
        Appointment asynthomaticList = newAppointmentList();
        char fiscalCode[FISCALCODE_SIZE];
        char *scansymptoms = NULL;
        int scanSlot;

        while (!feof(appointmentDB)) {
            if (fscanf(appointmentDB, "%s\t%d\t", fiscalCode, &scanSlot) != EOF) {
                scansymptoms = getSymptoms(appointmentDB);
                if (scansymptoms != NULL) {
                    timeSlot slot = (timeSlot) scanSlot;
                    if (strcmp(scansymptoms, "(null)") == 0) {
                        asynthomaticList = appointmentInsert(asynthomaticList, fiscalCode, slot, NULL);
                    }

                    else {
                        synthomaticList = appointmentInsert(synthomaticList, fiscalCode, slot, scansymptoms);
                    }
                }
            }
        }
        appList = appointmentAppend(synthomaticList, asynthomaticList);
        fclose(appointmentDB);
    }
    else {
        //TODO: Handle file opening error
    }
    return appList;
}

void saveAppointmentListBody(Appointment appList, FILE * appointmentDB) {
    if (appointmentDB != NULL) {
        if (appList != NULL) {
            if (fprintf(appointmentDB, "%s\t%d\t%s\n", appList->fiscalCode, appList->slot, appList->symptoms) < 0) {
            // TODO: Handle error in writing
            }
            else {
                saveAppointmentListBody(appList->next, appointmentDB);
            }
        }
    }
}

void saveAppointmentList(Appointment appList) {
    FILE * appointmentDB = fopen(APPOINTMENT_DB, "w");
    if (appointmentDB != NULL) {
        saveAppointmentListBody(appList, appointmentDB);
    }
    else {
        //TODO: Handle error file opening
    }
}

// GESTIONE DB TEST
