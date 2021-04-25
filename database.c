#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "database.h"
#include "const.h"
#include "helper.h"

#include "s_patient.h"
#include "s_quarantine.h"



/* GESTIONE DB PAZIENTI */

/* Scrive su file (in modalit� append) i campi di un elemento Patient */
void savePatient(char fiscalCode[], char password[]) {
    FILE * patientDB;
    patientDB = fopen(PATIENT_DB, "a");
    if (patientDB != NULL) {
        if (fprintf(patientDB, "%s\t%s\n", fiscalCode, password) < 0) {
            printf("SAVEPATIENT: ");
            printMessage(ERR_WRITING);
        }
        fclose(patientDB);
    }
    else {
        printf("SAVEPATIENT: ");
        printMessage(ERR_FILEACCESS);
    }

}

/* Carica da file una lista di elementi Patient */
Patient loadPatientList() {
    Patient ptList = patientNewList();
    FILE * patientDB = fopen(PATIENT_DB, "r");
    if (patientDB != NULL) {
        char fiscalCode[FISCALCODE_SIZE];
        char password[PASSWORD_SIZE];
        while (!feof(patientDB)) {
            if (fscanf(patientDB, "%s %s", fiscalCode, password) != EOF) {
                ptList = patientTailInsert(ptList, fiscalCode, password);
            }
        }
        fclose(patientDB);
    }

    return ptList;
}


/* GESTIONE DB LAVORATORI LABORATORIO */

/* Carica da file una lista di elementi Employee */
Employee loadEmployeeList() {
    Employee emList = employeeNewList();
    FILE * employeeDB = fopen(EMPLOYEE_DB, "r");
    if (employeeDB != NULL) {
        if (ftell(employeeDB) == 0) {
            fclose(employeeDB);
            emList = loadSampleEmployeeList();
            saveEmployeeList(emList);
        }
        else {
            int id;
            char password[PASSWORD_SIZE];

            while (!feof(employeeDB)) {
                if(fscanf(employeeDB, "%d\t%s\n", &id, password) != EOF) {
                    emList = employeeTailInsert(emList, id, password);
                }
            }
            fclose(employeeDB);
        }
    }
    else {
        emList = loadSampleEmployeeList();
        saveEmployeeList(emList);
    }

    return emList;
}

/* Funzione ricorsiva di appoggio utilizzata in saveEmployeeList */
void saveEmployeeListBody(Employee emList, FILE * file) {
    if (emList != NULL) {
        if (file != NULL) {
            if (fprintf(file, "%d\t%s\n", emList->id, emList->password) > 0) {
                saveEmployeeListBody(emList->next, file);
            }
            else {
                printf("SAVEEMPLOYEELIST: ");
                printMessage(ERR_WRITING);
            }
        }
    }
}

/* Salva su file una lista di elementi Employee */
void saveEmployeeList(Employee emList) {
    FILE * employeeDB = fopen(EMPLOYEE_DB, "w");
    if (employeeDB != NULL) {
        saveEmployeeListBody(emList, employeeDB);
        fclose(employeeDB);
    }
    else {
        printf("SAVEEMPLOYEELIST: ");
        printMessage(ERR_WRITING);
    }
}


/* GESTIONE DB APPUNTAMENTI */

/* Salva su file (in mod. append) i campi di un elemento Appointment */
void saveAppointment(Appointment apNode, FILE * file) {
    if (apNode != NULL) {
        if (file == NULL) {
            file = fopen(APPOINTMENT_DB, "a");
        }

        if (file != NULL) {
            if (fprintf(file, "%s\t%d\t", apNode->fiscalCode, apNode->slot) < 0) {
                printf("SAVEAPPOINTMENT (1): ");
                printMessage(ERR_WRITING);
            }

            if (strcmp(apNode->symptoms, "") == 0) {
                if (fprintf(file, "(null)\n") < 0) {
                    printf("SAVEAPPOINTMENT (2): ");
                    printMessage(ERR_WRITING);
                }
            } else {
                if (fprintf(file, "%s\n", apNode->symptoms) < 0) {
                    printf("SAVEAPPOINTMENT (3): ");
                    printMessage(ERR_WRITING);
                }
            }
            fclose(file);
        }

        else {
            printMessage("SAVEAPPOINTMENT: The database does not exist or could not be opened.");
        }
    }
}

/* Carica da file una lista di elementi Appointment */
Appointment loadAppointmentList() {
    Appointment apList = appointmentNewList();
    FILE * appointmentDB = fopen(APPOINTMENT_DB, "r");
    if (appointmentDB != NULL) {
        /* Per assicurare la priorit� e la struttura FIFO, gli Appointment sono smistati in due liste durante la lettura da file */
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

        /* E poi concatenati e restituiti */
        apList = appointmentAppendToList(symptomaticList, asymptomaticList);
        fclose(appointmentDB);
    }

    return apList;
}

/* Funzione ricorsiva di appoggio utilizzata in saveAppointmentList */
void saveAppointmentListBody(Appointment apList, FILE * appointmentDB) {
    if (appointmentDB != NULL) {
        if (apList != NULL) {
            saveAppointment(apList, appointmentDB);
            saveAppointmentListBody(apList->next, appointmentDB);
        }
    }
    else {
        printf("SAVEAPPOINTMENTLIST: ");
        printMessage(ERR_FILEACCESS);
    }
}

/* Salva su file una lista di elementi Appointment */
void saveAppointmentList(Appointment apList) {
    FILE * appointmentDB = fopen(APPOINTMENT_DB, "w");
    if (appointmentDB != NULL) {
        saveAppointmentListBody(apList, appointmentDB);
        fclose(appointmentDB);
    }
    else {
        printf("SAVEAPPOINTMENTLIST: ");
        printMessage(ERR_FILEACCESS);
    }
}

/* Elimina il contenuto del file appointment_db.txt */
void dropAppointmentDB() {
    FILE * appointmentDB = fopen(APPOINTMENT_DB, "w");
    if (appointmentDB != NULL) {
        fclose(appointmentDB);
    }
    else {
        printf("DROPAPPOINTMENTDB: ");
        printMessage(ERR_FILEACCESS);
    }
}


/* GESTIONE DB RISULTATI TEST */

/* Carica da file una lista di elementi TestResult */
TestResult loadTestResultList() {
    TestResult rsList = testResultNewList();
    FILE * testResultDB = fopen(TESTRESULT_DB, "r");
    if (testResultDB != NULL) {
        char fiscalCode[FISCALCODE_SIZE];
        char response[RESPONSE_SIZE];
        int day;

        while (!feof(testResultDB)) {
            if(fscanf(testResultDB, "%s\t%s\t%d\n", fiscalCode, response, &day) != EOF) {
                rsList = testResultTailInsert(rsList, fiscalCode, response, day);
            }
            else {
                printf("LOADTESTRESULTLIST: ");
                printMessage(ERR_READING);
            }
        }
        fclose(testResultDB);
    }

    return rsList;
}

/* Funzione ricorsiva di appoggio utilizzata in saveTestResultList */
void saveTestResultListBody(Appointment apList, FILE * testResultDB, int currentDay, Quarantine *qtList) {
    if (testResultDB != NULL) {
        if (apList != NULL) {
            if (fprintf(testResultDB, "%s\t", apList->fiscalCode) < 0) {
                printf("SAVETESTRESULTLIST: ");
                printMessage(ERR_WRITING);
            }
            else {
                char response[RESPONSE_SIZE];
                if (generateTestResult()) {
                    (*qtList) = quarantineTailInsert((*qtList), apList->fiscalCode);
                    strcpy(response, "POSITIVE");
                }
                else{
                    (*qtList) = quarantineDeleteNodeByFiscalCode((*qtList), apList->fiscalCode);
                    strcpy(response, "NEGATIVE");
                }

                if (fprintf(testResultDB, "%s\t", response) < 0) {
                    printf("SAVETESTRESULTLIST: ");
                    printMessage(ERR_WRITING);
                }
                else {
                    if (fprintf(testResultDB, "%d\n", currentDay) < 0) {
                        printf("SAVETESTRESULTLIST: ");
                        printMessage(ERR_WRITING);
                    }
                    else {
                        saveTestResultListBody(apList->next, testResultDB, currentDay, qtList);
                    }
                }
            }
        }
    }
    else {
        printf("SAVETESTRESULTLIST: ");
        printMessage(ERR_FILEACCESS);
    }
}

/* Salva su file (in mod. append) un elemento Reservation convertendo ogni Appointment in un TestResult */
void saveTestResultList(Reservation res, Quarantine *qtList) {
    FILE * testResultDB = fopen(TESTRESULT_DB, "a");
    if (testResultDB != NULL) {
        saveTestResultListBody(res->morning, testResultDB, res->currentDay, qtList);
        saveTestResultListBody(res->afternoon, testResultDB, res->currentDay, qtList);
        saveTestResultListBody(res->evening, testResultDB, res->currentDay, qtList);
        fclose(testResultDB);
    }
    else {
        printf("SAVETESTRESULTLIST: ");
        printMessage(ERR_FILEACCESS);
    }
}



/* DB QUARANTENA */

/* Carica da file una lista di elementi Quarantine */
Quarantine loadQuarantineList() {
    Quarantine qtList = quarantineNewList();
    FILE * quarantineDB = fopen(QUARANTINE_DB, "r");
    if (quarantineDB != NULL) {
        char fiscalCode[FISCALCODE_SIZE];
        while (!feof(quarantineDB)) {
            if (fscanf(quarantineDB, "%s", fiscalCode) != EOF) {
                qtList = quarantineTailInsert(qtList, fiscalCode);
            }
        }
        fclose(quarantineDB);
    }

    return qtList;
}

/* Funzione ricorsiva di appoggio utilizzata in saveQuarantineList */
void saveQuarantineListBody(Quarantine qtList, FILE * file) {
    if (file != NULL) {
        if (qtList != NULL) {
            if(fprintf(file, "%s\n", qtList->fiscalCode) > 0) {
                saveQuarantineListBody(qtList->next, file);
            }
            else {
                printf("SAVEQUARANTINELIST: ");
                printMessage(ERR_WRITING);
            }
        }
    }
    else {
        printf("SAVEQUARANTINELIST: ");
        printMessage(ERR_FILEACCESS);
    }
}

/* Salva su file una lista di elementi Quarantine */
void saveQuarantineList(Quarantine qtList) {
    FILE * quarantineDB = fopen(QUARANTINE_DB, "w");
    if (quarantineDB != NULL) {
        saveQuarantineListBody(qtList, quarantineDB);
    }
    else {
        printf("SAVEQUARANTINELIST: ");
        printMessage(ERR_FILEACCESS);
    }
}

