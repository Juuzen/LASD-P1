#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
