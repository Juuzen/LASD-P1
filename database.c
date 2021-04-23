#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "database.h"
#include "const.h"
#include "structure.h"
#include "s_patient.h"
#include "s_quarantine.h"
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
    Patient ptList = patientNewList();
    FILE * patient_db = fopen(PATIENT_DB, "r");
    if (patient_db != NULL) {
        char fiscalCode[FISCALCODE_SIZE];
        char password[PASSWORD_SIZE];
        while ((fscanf(patient_db, "%s %s", fiscalCode, password)) != EOF) {

            ptList = patientTailInsert(ptList, fiscalCode, password);
        }
    } else {
        printf("LOAD: The DB could not be opened.\n");
        //TODO: Handle file opening error
    }
    fclose(patient_db);
    return ptList;
}


/* GESTIONE DB LAVORATORI LABORATORIO */
Employee loadEmployeeList() {
    Employee emList = employeeNewList();
    FILE * employeeDB = fopen(EMPLOYEE_DB, "r");
    if (employeeDB != NULL) {
        int id;
        char password[PASSWORD_SIZE];

        while (!feof(employeeDB)) {
            if(fscanf(employeeDB, "%d\t%s\n", &id, password) != EOF) {
                emList = employeeTailInsert(emList, id, password);
            }
        }
        fclose(employeeDB);
    }
    return emList;
}


/* GESTIONE DB APPUNTAMENTI */
void saveAppointment(Appointment apNode) {
    if (apNode != NULL) {
    FILE * appointmentDB;
    appointmentDB = fopen(APPOINTMENT_DB, "a");
    if (appointmentDB != NULL) {
        if (fprintf(appointmentDB, "%s\t%d\t", apNode->fiscalCode, apNode->slot) < 0) {
            // TODO: Handle error in writing
        }

        if (strcmp(apNode->symptoms, "") == 0) {
            if (fprintf(appointmentDB, "(null)\n") < 0) {
                //TODO: Handle error in writing
            }
        } else {
            if (fprintf(appointmentDB, "%s\n", apNode->symptoms) < 0) {
                //TODO: Handle error in writing
            }
        }
    }
    else {
        printf ("SAVE: The DB could not be opened.\n");
    }
    fclose(appointmentDB);
    }
}

Appointment loadAppointmentList() {
    Appointment apList = appointmentNewList();
    FILE * appointmentDB = fopen(APPOINTMENT_DB, "r");
    if (appointmentDB != NULL) {
        Appointment symptomaticList = appointmentNewList();
        Appointment asymptomaticList = appointmentNewList();
        char fiscalCode[FISCALCODE_SIZE];
        char *scansymptoms = NULL;
        int scanSlot;

        while (!feof(appointmentDB)) {
            if (fscanf(appointmentDB, "%s\t%d\t", fiscalCode, &scanSlot) != EOF) {
                timeSlot slot = (timeSlot) scanSlot;

                scansymptoms = getSymptoms(appointmentDB);
                if (scansymptoms != NULL) {

                    if (strcmp(scansymptoms, "(null)") == 0)
                        asymptomaticList = appointmentTailInsert(asymptomaticList, fiscalCode, slot, NULL);

                    else symptomaticList = appointmentTailInsert(symptomaticList, fiscalCode, slot, scansymptoms);
                }

                else asymptomaticList = appointmentTailInsert(asymptomaticList, fiscalCode, slot, NULL);
            }
        }

        apList = appointmentAppendToList(symptomaticList, asymptomaticList);
        fclose(appointmentDB);
    }
    else {
        //TODO: Handle file opening error
    }
    return apList;
}

void saveAppointmentListBody(Appointment apList, FILE * appointmentDB) {
    if (appointmentDB != NULL) {
        if (apList != NULL) {
            if (fprintf(appointmentDB, "%s\t%d\t%s\n", apList->fiscalCode, apList->slot, apList->symptoms) < 0) {
            // TODO: Handle error in writing
            }
            else {
                saveAppointmentListBody(apList->next, appointmentDB);
            }
        }
    }
}

void saveAppointmentList(Appointment apList) {
    if (apList != NULL) {
        FILE * appointmentDB = fopen(APPOINTMENT_DB, "w");
        if (appointmentDB != NULL) {
            saveAppointmentListBody(apList, appointmentDB);
            fclose(appointmentDB);
        }
        else {
        //TODO: Handle error file opening
        }
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

void saveTestResultListBody(Appointment apList, FILE * testResultDB, int currentDay) {
    if (testResultDB != NULL) {
        if (apList != NULL) {
            if (fprintf(testResultDB, "%s\t", apList->fiscalCode) < 0) {
                //TODO: Gestire errore in scrittura
            }
            else {
                char response[RESPONSE_SIZE];
                if (generateTestResult()) strcpy(response, "POSITIVE");
                else strcpy(response, "NEGATIVE");

                if (fprintf(testResultDB, "%s\t", response) < 0) {
                    //TODO: Gestire errore in scrittura
                }
                else {
                    if (fprintf(testResultDB, "%d\n", currentDay) < 0) {
                        //TODO: Gestire errore in scrittura
                    }
                    else {
                        saveTestResultListBody(apList->next, testResultDB, currentDay);
                    }
                }
            }
        }
    }
}

void saveTestResultList(Reservation res) {
    if (res != NULL) {
        FILE * testResultDB = fopen(TESTRESULT_DB, "a");
        if (testResultDB != NULL) {
            saveTestResultListBody(res->morning, testResultDB, res->currentDay);
            saveTestResultListBody(res->afternoon, testResultDB, res->currentDay);
            saveTestResultListBody(res->evening, testResultDB, res->currentDay);
            fclose(testResultDB);
        }

        else {
            //TODO: Handle error in opening file
        }
    }
}


/* DB QUARANTENA */
Quarantine loadQuarantineList() {
    Quarantine list = NULL;
    FILE * quarantineDB = fopen(QUARANTINE_DB, "r");
    if (quarantineDB != NULL) {

    }
    return list;
}
