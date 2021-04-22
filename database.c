#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "database.h"
#include "const.h"
#include "structure.h"
#include "helper.h"

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

// GESTIONE DB LAVORATORI LABORATORIO

LabWorker loadLabWorkers() {
    LabWorker wkList = newLabWorkerList();
    FILE * labWorkerDB = fopen(LABWORKER_DB, "r");
    if (labWorkerDB != NULL) {
        int workerId;
        char password[PASSWORD_SIZE];

        while (!feof(labWorkerDB)) {
            if(fscanf(labWorkerDB, "%d\t%s\n", &workerId, password) != EOF) {
                wkList = labWorkerInsert(wkList, workerId, password);
            }
        }
        fclose(labWorkerDB);
    }
    return wkList;
}



// GESTIONE DB APPUNTAMENTI

void saveAppointment(Appointment app) {
    FILE * appointmentDB;
    appointmentDB = fopen(APPOINTMENT_DB, "a");
    if (appointmentDB != NULL) {
        if (fprintf(appointmentDB, "%s\t%d\t", app->fiscalCode, app->slot) < 0) {
            // TODO: Handle error in writing
        }

        if (strcmp(app->symptoms, "") == 0) {
            if (fprintf(appointmentDB, "(null)\n") < 0) {
                //TODO: Handle error in writing
            }
        } else {
            if (fprintf(appointmentDB, "%s\n", app->symptoms) < 0) {
                //TODO: Handle error in writing
            }
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
        fclose(appointmentDB);
    }
    else {
        //TODO: Handle error file opening
    }

}
void dropAppointmentDB() {
    FILE * appointmentDB = fopen(APPOINTMENT_DB, "w");
    if (appointmentDB != NULL) {
        fclose(appointmentDB);
    }
}

// GESTIONE DB TEST

int getCurrentDay() {
    FILE * fp = fopen(TESTRESULT_DB, "r");

    if (fp != NULL) {
        fseek(fp, -1, SEEK_END);
        char c;
        do {
            fseek(fp, -2, SEEK_CUR);
            c = fgetc(fp);

        } while (c != '\t');

        int lastDay;
        if (fscanf(fp, "%d", &lastDay) > 0) {
            lastDay += 1;
        }
        else {
            //TODO: Gestire errore in lettura del giorno
        }
        fclose(fp);
        return lastDay;
    }
    else {
        //FIXME: Gestire fopen quando il file non esiste (crearlo? restituire 1?)
        return 1;
    }
}



// GESTIONE ESITI TEST
TestResult loadTestResults() {
    TestResult rsList = newTestResultList();
    FILE * testResultDB = fopen(TESTRESULT_DB, "r");
    if (testResultDB != NULL) {
        char fiscalCode[FISCALCODE_SIZE];
        char response[RESPONSE_SIZE];
        int day;

        while (!feof(testResultDB)) {
            if(fscanf(testResultDB, "%s\t%s\t%d\n", fiscalCode, response, &day) != EOF) {
                rsList = testResultInsert(rsList, fiscalCode, response, day);
            }
        }
        fclose(testResultDB);
    }

    return rsList;
}
void saveTestResultListBody(Appointment appList, FILE * testResultDB, int currentDay) {
    if (testResultDB != NULL) {
        if (appList != NULL) {
            if (fprintf(testResultDB, "%s\t", appList->fiscalCode) < 0) {

            }
            else {
                char response[RESPONSE_SIZE];
                if (generateTestResult()) strcpy(response, "POSITIVE");
                else strcpy(response, "NEGATIVE");
                if (fprintf(testResultDB, "%s\t", response) < 0) {

                }
                else {
                    if (fprintf(testResultDB, "%d\n", currentDay) < 0) {

                    }
                    else {
                        saveTestResultListBody(appList->next, testResultDB, currentDay);
                    }
                }
            }
        }
    }
}
void saveTestResultList(TestReservation testList) {
    FILE * testResultDB = fopen(TESTRESULT_DB, "a");
    if (testResultDB != NULL) {
        saveTestResultListBody(testList->morning, testResultDB, testList->currentDay);
        saveTestResultListBody(testList->afternoon, testResultDB, testList->currentDay);
        saveTestResultListBody(testList->evening, testResultDB, testList->currentDay);
        fclose(testResultDB);
    }

    else {
        //TODO: Handle error in opening file
    }
}
















