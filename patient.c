#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "database.h"
#include "patient.h"
#include "s_patient.h"
#include "s_appointment.h"
#include "s_result.h"
#include "helper.h"

/* FUNZIONI AUSILIARIE */
bool patientloginCheck(Patient ptList, char fiscalCode[], char password[]) {
    if (ptList == NULL) {
        return false;
    }
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

bool isPatientRegistered(Patient ptList, char fiscalCode[]) {
    if (ptList == NULL) {
        return false;
    }

    else {
        return ((strcmp(ptList->fiscalCode, fiscalCode)) == 0) ? true : isPatientRegistered(ptList->next, fiscalCode);
    }
}

bool patientRegister(Patient* ptList, char fiscalCode[], char password[]) {
    bool response = false;
    if (!isPatientRegistered(*ptList, fiscalCode)) {
        *ptList = patientTailInsert(*ptList, fiscalCode, password);
        savePatient(fiscalCode, password);
        response = true;
    }

    return response;
}

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

void patientDeleteAppointment(Appointment* apList, char fiscalCode[]) {
    *apList = appointmentDeleteByFiscalCode(*apList, fiscalCode);
    saveAppointmentList(*apList);
}


/* FUNZIONI UI */
void patientShowTestResultsUi(char fiscalCode[]) {
    TestResult rsList = loadTestResultList();
    clearScreen();
    if (rsList == NULL) {
        printf("You took 0 tests up to this moment.\n");
        printMessage(PAUSE_DEFAULT);
    }
    else {
        printf("Here are your test results:\n");
        /* Di default, vengono stampati i test in senso anti-cronologico */
        testResultPrintByFiscalCode(rsList, fiscalCode, false);
        printMessage(PAUSE_DEFAULT);
    }
}

void patientAppointmentRequestUi(Appointment apList, char fiscalCode[]) {
    int userChoice = -1;
    Appointment found = appointmentFindByFiscalCode(apList, fiscalCode);
    if (found != NULL) {
        printf("You already have an appointment with the following informations:\n");
        appointmentPrintNode(found);
        printMessage(PAUSE_DEFAULT);
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
        fflush(stdin);
        if (userChoice != 4) {
            timeSlot slot = (timeSlot) --userChoice;

            printf("If you have any symptoms, please provide a concise explanation (max %d characters):\n", SYMPTOMS_SIZE);
            char * symptoms = getSymptoms(stdin);

            bool response = patientRequestAppointment(&apList, fiscalCode, slot, symptoms);
            if (response) printf("Appointment requested!\n");
            else printf("There was a problem in requesting the appointment. Please try again later.\n");
            printMessage(PAUSE_DEFAULT);
        }
    }

}

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

void patientDeleteAppointmentUi(Appointment* apList, char fiscalCode[]) {
    clearScreen();
    Appointment app = appointmentFindByFiscalCode(*apList, fiscalCode);
    appointmentPrintNode(app);

    if (app == NULL) {
        printf("You have no appointments up to now.\n");
        printMessage(PAUSE_DEFAULT);
    }
    else {
        char input;
        do {
            clearScreen();
            printf("Here is you appointment informations:\n");
            appointmentPrintNode(app);
            printf("Do you want to delete this appointment? [y/n] ");
            fflush(stdin);
            scanf("%c", &input);
            fflush(stdin);
            if ((input == 'y') || (input == 'Y')) {
                patientDeleteAppointment(apList, fiscalCode);
            }

            else if ((input == 'n') || (input == 'N')) {
                printf("No changes will be made.\n");
                printMessage(PAUSE_DEFAULT);
            }

            else {
                printf("Only 'y' or 'n' are permitted (case in-sensitive).\n");
                printMessage(PAUSE_DEFAULT);
            }
        } while ((input != 'y') && (input != 'Y') && (input != 'n') && (input != 'N'));
    }
}

void patientAccountUi(Reservation *res, char fiscalCode[]) {
    int userChoice = -1;
    bool running = true;
    Appointment appList = loadAppointmentList();

    do {
        do {
            clearScreen();
            printf("Welcome %s.\n", fiscalCode);
            printf("Please make a choice:\n\n");
            printf("1. REQUEST A COVID TEST APPOINTMENT\n");
            printf("2. CHECK YOUR APPOINTMENTS\n");
            printf("3. CANCEL AN APPOINTMENT\n");
            printf("4. SHOW THE RESULTS FOR PREVIOUS TESTS\n");
            printf("5. GO BACK\n");
            printf("\nYour choice: ");

            userChoice = getChoice(5);
        } while (userChoice == -1);

        switch(userChoice) {
            case 1:
                patientAppointmentRequestUi(appList, fiscalCode);
                break;
            case 2:
                patientShowReservationUi(res, fiscalCode);
                break;
            case 3:
                patientDeleteAppointmentUi(&appList, fiscalCode);
                break;
            case 4:
                patientShowTestResultsUi(fiscalCode);
                break;
            case 5:
                printf("I'm now halting.\n");
                running = false;
                break;
            default:
                printf("Something went wrong...\n");
                // FIXME: Handle shutdown
                break;
        }
    } while (running);

    appointmentFreeList(appList);
}

void patientLoginUi(Reservation *res, Patient ptList) {

    int userChoice = -1;
    bool running = true;
    char fiscalCode[FISCALCODE_SIZE];
    char password[PASSWORD_SIZE];
    do {
        clearScreen();
        printf("Please provide your fiscal code: ");
        scanf("%16s", fiscalCode);
        fflush(stdin);
        printf("Please provide your password: ");
        //TODO: masked scanf
        scanf("%20s", password);
        fflush(stdin);

        if (patientloginCheck(ptList, fiscalCode, password)) {
            //login authorized
            patientAccountUi(res, fiscalCode);
            running = false;
        }
        else {
            do {
                //credentials are not correct
                //you can choose to stop and go back
                printf("Your fiscal code and/or password is incorrect.\n");
                printf("What do you want to do? Please make a choice:\n");
                printf("1. TRY AGAIN\n");
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
                    // TODO: Handle error
            }
        }
    } while (running);
}

void patientRegisterUi(Patient ptList) {
    int userChoice = -1;
    bool running = true;
    char fiscalCode[FISCALCODE_SIZE];
    char password[PASSWORD_SIZE];
    do {
        clearScreen();
        printf("Please provide your fiscal code: ");
        scanf("%16s", fiscalCode);
        fflush(stdin);
        printf("Please choose a password: ");
        //masked scanf
        scanf("%20s", password);
        fflush(stdin);

        if (patientRegister(&ptList, fiscalCode, password)) {
            printf("You are now registered to the platform!\n");
            printMessage(PAUSE_DEFAULT);
            running = false;
        }
        else {
            do {
                //that fiscal code was already present in db
                //you can choose to stop and go back
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
                    // TODO: Handle error
            }
        }
    } while (running);
}

void patientMainMenuUi(Reservation *res) {
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
                patientLoginUi(res, ptList);
                break;
            case 2:
                patientRegisterUi(ptList);
                break;
            case 3:
                printf("I'm now halting.\n");
                running = false;
                break;
            default:
                printf("Something went wrong...\n");
                // FIXME: Handle shutdown
                break;
        }
    } while (running);

    patientFreeList(ptList);
}

