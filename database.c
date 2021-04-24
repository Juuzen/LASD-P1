#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "database.h"
#include "const.h"

#include "s_patient.h"
#include "s_quarantine.h"
#include "helper.h"


/* GESTIONE DB PAZIENTI */

/* Scrive su file (in modalità append) i campi di un elemento Patient */
void savePatient(char fiscalCode[], char password[]) {
    FILE * patientDB;
    patientDB = fopen(PATIENT_DB, "a");
    if (patientDB != NULL) {
        //TODO: errore in scrittura
        fprintf(patientDB, "%s\t%s\n", fiscalCode, password);
    }
    else {
        pause("SAVEPATIENT: The DB does not exist or could not be opened.");
    }
    fclose(patientDB);
}

/* Carica da file una lista di elementi Patient */
Patient loadPatientList() {
    Patient ptList = patientNewList();
    FILE * patient_db = fopen(PATIENT_DB, "r");
    if (patient_db != NULL) {
        char fiscalCode[FISCALCODE_SIZE];
        char password[PASSWORD_SIZE];
        while ((fscanf(patient_db, "%s %s", fiscalCode, password)) != EOF) {

            ptList = patientTailInsert(ptList, fiscalCode, password);
        }
        fclose(patient_db);
    } else {
        pause("LOADPATIENTLIST: The database does not exist or could not be opened.");
    }

    return ptList;
}


/* GESTIONE DB LAVORATORI LABORATORIO */

/* Carica da file una lista di elementi Employee */
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
    else {
        pause("LOADEMPLOYEELIST: The database does not exist or could not be opened.");
    }
    return emList;
}


/* GESTIONE DB APPUNTAMENTI */

/* Salva su file (in modalità append) i campi di un elemento Appointment */
void saveAppointment(Appointment apNode, FILE * file) {
    if (apNode != NULL) {
        if (file == NULL) {
            file = fopen(APPOINTMENT_DB, "a");
        }

        if (file != NULL) {
            if (fprintf(file, "%s\t%d\t", apNode->fiscalCode, apNode->slot) < 0) {
                pause("SAVEAPPOINTMENT: Error in writing the appointment.");
            }

            if (strcmp(apNode->symptoms, "") == 0) {
                if (fprintf(file, "(null)\n") < 0) {
                    pause("SAVEAPPOINTMENT: Error in writing the appointment.");
                }
            } else {
                if (fprintf(file, "%s\n", apNode->symptoms) < 0) {
                    pause("SAVEAPPOINTMENT: Error in writing the appointment.");
                }
            }
            fclose(file);
        }

        else {
            pause("SAVEAPPOINTMENT: The database does not exist or could not be opened.");
        }
    }
}

/* Carica da file una lista di elementi Appointment */
Appointment loadAppointmentList() {
    Appointment apList = appointmentNewList();
    FILE * appointmentDB = fopen(APPOINTMENT_DB, "r");
    if (appointmentDB != NULL) {
        /* Per assicurare la priorità e la struttura FIFO, gli Appointment sono smistati in due liste durante la lettura da file */
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
            else {
                pause("LOADAPPOINTMENTLIST: Error in reading from file.");
            }
        }

        /* E poi concatenati e restituiti */
        apList = appointmentAppendToList(symptomaticList, asymptomaticList);
        fclose(appointmentDB);
    }
    else {
        pause("LOADAPPOINTMENTLIST: The database does not exist or could not be opened.");
    }
    return apList;
}

/* Funzione ricorsiva di appoggio utilizzata in saveAppointmentList*/
void saveAppointmentListBody(Appointment apList, FILE * appointmentDB) {
    if (appointmentDB != NULL) {
        if (apList != NULL) {
            saveAppointment(apList, appointmentDB);
            saveAppointmentListBody(apList->next, appointmentDB);
        }
    }
    else {
        pause("SAVEAPPOINTMENTLIST: The database does not exist or could not be opened.");
    }
}

/* Salva su file una lista di elementi Appointment */
void saveAppointmentList(Appointment apList) {
    if (apList != NULL) {
        FILE * appointmentDB = fopen(APPOINTMENT_DB, "w");
        if (appointmentDB != NULL) {
            saveAppointmentListBody(apList, appointmentDB);
            fclose(appointmentDB);
        }
        else {
            pause("SAVEAPPOINTMENTLIST: The database does not exist or could not be opened.");
        }
    }


}

/* Elimina il contenuto del file appointment_db.txt */
void dropAppointmentDB() {
    FILE * appointmentDB = fopen(APPOINTMENT_DB, "w");
    if (appointmentDB != NULL) {
        fclose(appointmentDB);
    }
    else {
        pause("DROPAPPOINTMENTDB: The database does not exist or could not be opened.");
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
        }
        fclose(testResultDB);
    }
    else {
        pause("LOADTESTRESULTLIST: The database does not exist or could not be opened.");
    }

    return rsList;
}

/* Funzione ricorsiva di appoggio utilizzata in saveTestResultList */
void saveTestResultListBody(Appointment apList, FILE * testResultDB, int currentDay) {
    if (testResultDB != NULL) {
        if (apList != NULL) {
            if (fprintf(testResultDB, "%s\t", apList->fiscalCode) < 0) {
                pause("SAVETESTRESULTLIST: Error in writing into the file.");
            }
            else {
                char response[RESPONSE_SIZE];
                if (generateTestResult()) strcpy(response, "POSITIVE");
                else strcpy(response, "NEGATIVE");

                if (fprintf(testResultDB, "%s\t", response) < 0) {
                    pause("SAVETESTRESULTLIST: Error in writing into the file.");
                }
                else {
                    if (fprintf(testResultDB, "%d\n", currentDay) < 0) {
                        pause("SAVETESTRESULTLIST: Error in writing into the file.");
                    }
                    else {
                        saveTestResultListBody(apList->next, testResultDB, currentDay);
                    }
                }
            }
        }
    }
    else {
        pause("SAVETESTRESULTLIST: The database does not exist or could not be opened.");
    }
}

/* Salva su file (in modalità append) un elemento Reservation convertendo ogni Appointment in un TestResult */
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
        pause("SAVETESTRESULTLIST: The database does not exist or could not be opened.");
    }
}
}


/* DB QUARANTENA */

/* Carica da file una lista di elementi Quarantine */
Quarantine loadQuarantineList() {
    Quarantine list = NULL;
    FILE * quarantineDB = fopen(QUARANTINE_DB, "r");
    if (quarantineDB != NULL) {

    }
    return list;
}
