#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "database.h"
#include "patient.h"
#include "structure.h"
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
        *ptList = patientInsert(*ptList, fiscalCode, password);
        savePatient(fiscalCode, password);
        response = true;
    }

    return response;
}
bool patientRequestAppointment(Appointment* appList, char fiscalCode[], timeSlot slot, char symptoms[]) {
    bool response = false;
    Appointment newApp = newAppointmentNode(fiscalCode, slot, symptoms);
    if (newApp != NULL) {
        *appList = appointmentAppend(*appList, newApp);
        saveAppointment(newApp);
        response = true;
    }
    return response;
}
void patientDeleteAppointment(Appointment* appList, char fiscalCode[]) {
    *appList = deleteAppointmentByFiscalCode(*appList, fiscalCode);
    saveAppointmentList(*appList);
}


/* FUNZIONI UI */
void patientShowTestResultsUi(char fiscalCode[]) {
    TestResult rsList = loadTestResults();
    clearScreen();
    if (rsList == NULL) {
        //FIXME: Non c'è modo di capire se la lista è vuota perché non ci sono elementi o perché vi è stato un errore
        printf("You made 0 tests up to this moment.\n");
        pause("Press ENTER to go back...");
    }
    else {
        printf("Here are your test results:\n");
        // by default, anti-chronological order is used
        printTestResultByFiscalCode(rsList, fiscalCode, false);
        pause("Press ENTER to go back...");
    }
}
void patientAppointmentRequestUi(Appointment appList, char fiscalCode[]) {
    int userChoice = -1;
    Appointment found = findAppointmentByFiscalCode(appList, fiscalCode);
    if (found != NULL) {
        printf("You already have an appointment with the following informations:\n");
        printAppointmentNode(found);
        pause("Press ENTER key to go back...");
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

            bool response = patientRequestAppointment(&appList, fiscalCode, slot, symptoms);
            if (response) printf("Appointment requested!\n");
            else printf("There was a problem in requesting the appointment. Please try again later.\n");
            pause("Press ENTER key to go back...");
        }
    }

}
void patientShowReservationUi(TestReservation *test, char fiscalCode[]) {
    clearScreen();
    Appointment app = searchAppointmentByFiscalCode((*test), fiscalCode);
    if (app == NULL) printf("You have no reservations at the moment.\n");

    else {
        printf("Here is your test reservation:\n");
        printAppointmentNode(app);
    }
    pause("Press ENTER key to go back...");
}
void patientDeleteAppointmentUi(Appointment* appList, char fiscalCode[]) {
    clearScreen();
    Appointment app = findAppointmentByFiscalCode(*appList, fiscalCode);
    printAppointmentNode(app);

    if (app == NULL) pause("You have no appointments.\nPress any key to go back...");
    else {
        char input;
        do {
            clearScreen();
            printf("Here is you appointment informations:\n");
            printAppointmentNode(app);
            printf("Do you want to delete this appointment? [y/n] ");
            fflush(stdin);
            scanf("%c", &input);
            fflush(stdin);
            if ((input == 'y') || (input == 'Y')) {
                patientDeleteAppointment(appList, fiscalCode);
            }

            else if ((input == 'n') || (input == 'N')) {
                pause("The appointment will not be deleted.\nPress any key to go back...");
            }

            else {
                pause("Wrong choice! Only 'y' or 'n' are permitted.\nPress any key to continue...");
            }
        } while ((input != 'y') && (input != 'Y') && (input != 'n') && (input != 'N'));
    }
}
void patientAccountUi(TestReservation *test, char fiscalCode[]) {
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
                patientShowReservationUi(test, fiscalCode);
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

    deleteAppointmentList(appList);
}
void patientLoginUi(TestReservation *test, Patient ptList) {

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
            patientAccountUi(test, fiscalCode);
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
            //registration complete
            //TODO: add code for pausing the flow
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
void patientMainMenuUi(TestReservation *test) {
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
                patientLoginUi(test, ptList);
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

    deletePatientList(ptList);
}
