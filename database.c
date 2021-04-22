#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "database.h"
#include "const.h"
#include "structure.h"
#include "helper.h"

/* GESTIONE DB PAZIENTI */
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


/* GESTIONE DB LAVORATORI LABORATORIO */
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


/* GESTIONE DB APPUNTAMENTI */
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


/* GESTIONE DB RISULTATI TEST */
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
