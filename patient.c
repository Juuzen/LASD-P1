#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "database.h"
#include "patient.h"
#include "s_patient.h"
#include "s_appointment.h"
#include "s_result.h"
#include "s_quarantine.h"
#include "helper.h"

/* FUNZIONI AUSILIARIE */

/* Restituisce true se il login va a buon fine */
bool patientloginCheck(Patient ptList, char fiscalCode[], char password[]) {
    if (ptList == NULL) return false;

    else {
        if (strcmp(ptList->fiscalCode, fiscalCode) == 0) {
            if (strcmp(ptList->password, password) == 0) {
                return true;
            }
            else {
                return false;
            }
        }
        else return patientloginCheck(ptList->next, fiscalCode, password);
    }
}

/* Restituisce true se il paziente � gi� presente nel database */
bool isPatientRegistered(Patient ptList, char fiscalCode[]) {
    if (ptList == NULL) return false;

    else {
        if (strcmp(ptList->fiscalCode, fiscalCode) == 0) return true;
        else return isPatientRegistered(ptList->next, fiscalCode);
    }
}

/* Restituisce true se il processo di registrazione � avvenuto con successo */
bool patientRegister(Patient* ptList, char fiscalCode[], char password[]) {
    bool response = false;
    if (!isPatientRegistered(*ptList, fiscalCode)) {
        /* Se non � presente nel database, lo salva */
        *ptList = patientTailInsert(*ptList, fiscalCode, password);
        savePatient(fiscalCode, password);
        response = true;
    }

    return response;
}

/* Restituisce true se il processo di richiesta di appuntamento � avvenuto con successo */
bool patientRequestAppointment(Appointment* apList, char fiscalCode[], timeSlot slot, char symptoms[]) {
    bool response = false;
    Appointment newApp = appointmentNewNode(fiscalCode, slot, symptoms);
    if (newApp != NULL) {
        *apList = appointmentAppendToList(*apList, newApp);
        saveAppointment(newApp, NULL);
        response = true;
    }
    return response;
}

/* Cancella la richiesta di appuntamento del paziente con determinato codice fiscale */
void patientDeleteAppointment(Appointment* apList, char fiscalCode[]) {
    *apList = appointmentDeleteByFiscalCode(*apList, fiscalCode);
    saveAppointmentList(*apList);
}


/* FUNZIONI UI */

/* Funzione di UI per il punto A della traccia */
void patientAppointmentRequestUi(Appointment *apList, char fiscalCode[]) {
    int userChoice = -1;
    Appointment found = appointmentFindByFiscalCode(*apList, fiscalCode);
    clearScreen();
    if (found != NULL) {
        printf("You already have an appointment with the following informations:\n");
        appointmentPrintNode(found);
    }
    else {
        do {
            clearScreen();
            printf("You are now requesting an appointment for a COVID test. Please choose your time slot:\n");
            printf("1. MORNING\n");
            printf("2. AFTERNOON\n");
            printf("3. EVENING\n");
            printf("4. GO BACK\n");
            printf("Your choice: ");
            userChoice = getChoice(4);
        } while (userChoice == -1);
        if (userChoice != 4) {
            timeSlot slot = (timeSlot) --userChoice;

            printf("If you have any symptoms, please provide a concise explanation (max %d characters);\n", SYMPTOMS_SIZE);
            printf("Otherwise just press ENTER KEY to go further.\n");
            printf("Your symptoms: ");
            char * symptoms = getSymptoms(stdin);

            bool response = patientRequestAppointment(apList, fiscalCode, slot, symptoms);
            if (response) printf("Appointment requested!\n");
            else printf("There was a problem in requesting the appointment. Please try again later.\n");
        }
    }
    printMessage(PAUSE_DEFAULT);
}

/* Funzione di UI per il punto B della traccia */
void patientShowReservationUi(Reservation *res, char fiscalCode[]) {
    clearScreen();
    Appointment app = searchReservationByFiscalCode((*res), fiscalCode);
    if (app == NULL) printf("You have no reservations at the moment.\n");

    else {
        printf("Here is your test reservation:\n");
        appointmentPrintNode(app);
    }
    printMessage(PAUSE_DEFAULT);
}

/* Funzione di UI per il punto C della traccia */
void patientDeleteAppointmentUi(Appointment* apList, char fiscalCode[]) {
    clearScreen();
    Appointment app = appointmentFindByFiscalCode(*apList, fiscalCode);

    if (app == NULL) printf("You have no appointments up to now.\n");

    else {
        char input;
        do {
            clearScreen();
            printf("Here is you appointment informations:\n");
            appointmentPrintNode(app);
            printf("Do you want to delete this appointment? [y/n] ");
            scanf("%c", &input);
            flushStdin();
            if ((input == 'y') || (input == 'Y')) {
                patientDeleteAppointment(apList, fiscalCode);
                printf("The appointment is now canceled.\n");
            }

            else if ((input == 'n') || (input == 'N')) {
                printf("No changes will be made.\n");
            }

            else {
                printf("Only 'y' or 'n' are permitted (case in-sensitive).\n");
                printMessage(PAUSE_DEFAULT);
            }
        } while ((input != 'y') && (input != 'Y') && (input != 'n') && (input != 'N'));
    }
    printMessage(PAUSE_DEFAULT);
}

/* Funzione di UI per il punto D della traccia */
void patientShowTestResultsUi(char fiscalCode[], int currentDay) {
    clearScreen();
    if (currentDay == 1) {
        printf("You can choose this options from day 2 onwards.\n");
    }
    else {
        TestResult rsList = loadTestResultList();
        clearScreen();
        /* Se non sono stati ancora effettuati test */
        if (rsList == NULL) printf("You took 0 tests up to this moment.\n");

        else {
            /* Ricaviamo i test effettuati dal paziente */
            TestResult filteredList = testResultFilterByFiscalCode(rsList, fiscalCode);
            if (filteredList == NULL) printf("You took 0 tests up to this moment.\n");

            else {
                printf("Here are your test results:\n");
                /* Di default, vengono stampati i test in senso anti-cronologico */
                testResultPrintList(filteredList, false);
                testResultFreeList(filteredList);
            }
        }
        testResultFreeList(rsList);
    }
    printMessage(PAUSE_DEFAULT);
}

/* Funzione di UI per la pagina personale del paziente */
void patientAccountUi(Reservation *res, char fiscalCode[], bool quarantined) {
    int userChoice = -1;
    bool running = true;
    Appointment apList = loadAppointmentList();

    do {
        do {
            clearScreen();
            printf("Welcome %s.\n", fiscalCode);
            if(quarantined) {
                printf ("YOUR LAST TEST WAS POSITIVE, IT'S STRONGLY SUGGESTED TO TAKE ANOTHER APPOINTMENT.\n");
            }
            printf("Please make a choice:\n\n");
            printf("1. REQUEST A COVID TEST APPOINTMENT\n");
            printf("2. CHECK YOUR RESERVATION\n");
            printf("3. CANCEL AN APPOINTMENT\n");
            printf("4. SHOW THE RESULTS FOR PREVIOUS TESTS\n");
            printf("5. GO BACK\n");
            printf("\nYour choice: ");

            userChoice = getChoice(5);
        } while (userChoice == -1);

        switch(userChoice) {
            case 1:
                patientAppointmentRequestUi(&apList, fiscalCode);
                break;
            case 2:
                patientShowReservationUi(res, fiscalCode);
                break;
            case 3:
                patientDeleteAppointmentUi(&apList, fiscalCode);
                break;
            case 4:
                patientShowTestResultsUi(fiscalCode, (*res)->currentDay);
                break;
            case 5:
                running = false;
                break;
            default:
                break;
        }
    } while (running);

    appointmentFreeList(apList);
}

/* Funzione di UI per la pagina di login del paziente */
void patientLoginUi(Reservation *res, Patient ptList, Quarantine qtList) {

    int userChoice = -1;
    bool running = true;
    char fiscalCode[FISCALCODE_SIZE];
    char *password;
    do {
        clearScreen();
        printf("Please provide your fiscal code: ");
        scanf("%16s", fiscalCode);
        flushStdin();
        printf("Please provide your password (the echoing is turned off): ");
        password = maskedInput();

        if (patientloginCheck(ptList, fiscalCode, password)) {
            /* Accesso autorizzato */
            patientAccountUi(res, fiscalCode, quarantineFindPatientByFiscalCode(qtList, fiscalCode));
            running = false;
        }
        else {
            do {
                /* Credenziali non corrette */
                clearScreen();
                printf("Your fiscal code and/or password is incorrect.\n");
                printf("What do you want to do? Please make a choice:\n");
                printf("1. TRY AGAIN\n");
                printf("2. GO BACK\n");
                printf("\nYour choice: ");
                userChoice = getChoice(2);
            } while (userChoice == -1);

            if (userChoice == 2) running = false;
        }
    } while (running);
}

/* Funzione di UI per la pagina di registrazione del paziente */
void patientRegisterUi(Patient *ptList) {
    int userChoice = -1;
    bool running = true;
    char fiscalCode[FISCALCODE_SIZE];
    char * password;
    do {
        clearScreen();
        printf("Please provide your fiscal code: ");
        scanf("%16s", fiscalCode);
        flushStdin();
        printf("Please choose a password (the echoing is turned off): ");
        password = maskedInput();

        if (patientRegister(ptList, fiscalCode, password)) {
            clearScreen();
            printf("You are now registered to the platform!\n");
            printMessage(PAUSE_DEFAULT);
            running = false;
        }
        else {
            do {
                clearScreen();
                printf("A person with fiscal code %s was already present.\n", fiscalCode);
                printf("What do you want to do? Please make a choice:\n");
                printf("1. REPEAT THE REGISTRATION PROCESS\n");
                printf("2. GO BACK\n");
                printf("\nYour choice: ");
                userChoice = getChoice(2);
            } while (userChoice == -1);

            switch (userChoice) {
                case 1:
                    break;
                case 2:
                    running = false;
                    break;
                default:
                    break;
            }
        }
    } while (running);
}

/* Funzione di UI per la pagina principale del paziente */
void patientMainMenuUi(Reservation *res, Quarantine qtList) {
    int userChoice = -1;
    bool running = true;
    Patient ptList = loadPatientList();
    do {
        do {
            clearScreen();
            printf("Welcome to the COVID-19 testing centre - Patient Area.\n");
            printf("Please make a choice:\n\n");
            printf("1. LOGIN\n");
            printf("2. REGISTER\n");
            printf("3. GO BACK\n");
            printf("\nYour choice: ");

            userChoice = getChoice(3);
        } while (userChoice == -1);

        switch(userChoice) {
            case 1:
                patientLoginUi(res, ptList, qtList);
                running = false;
                break;
            case 2:
                patientRegisterUi(&ptList);
                break;
            case 3:
                running = false;
                break;
            default:
                break;
        }
    } while (running);

    patientFreeList(ptList);
}

