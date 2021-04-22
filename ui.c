#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ui.h"
#include "structure.h"
#include "patient.h"
#include "lab.h"
#include "helper.h"
#include "database.h"

/* UI LATO PAZIENTE */
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
        pause("Press any key to go back...");
    }
    else {
        do {
            clearScreen();
            printf("You are now requesting an appointment for a COVID test. Please choose your time slot:\n");
            printf("1. MORNING\n");
            printf("2. AFTERNOON\n");
            printf("3. EVENING\n");
            printf("4. EXIT\n");
            printf("Your choice: ");
            userChoice = getChoice(4);
        } while (userChoice == -1);

        if (userChoice != 4) {
            timeSlot slot = (timeSlot) userChoice;

            printf("If you have any symptoms, please provide a concise explanation (max %d characters):\n", SYMPTOMS_SIZE);
            char * symptoms = getSymptoms(stdin);

            bool response = patientRequestAppointment(&appList, fiscalCode, slot, symptoms);
            if (response) pause("Appointment requested!\nPress any key to go back...");
            else pause("There was a problem in requesting the appointment.\nPress any key to go back...");
        }
    }

}

void patientShowAppointmentUi(Appointment appList, char fiscalCode[]) {
    clearScreen();
    Appointment app = findAppointmentByFiscalCode(appList, fiscalCode);
    if (app == NULL) pause("You have no appointments.\nPress any key to go back...");
    else {
        printf("Here is you appointment informations:\n");
        printAppointmentNode(app);
        pause("Press any key to go back...");
    }
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
            scanf("%c", &input);
            if ((input == 'y') || (input == 'Y')) {
                patientDeleteAppointment(appList, fiscalCode);
            }

            else if ((input == 'n') || (input == 'N')) {
                pause("The appointment will not be deleted.\nPress any key to go back...");
            }

            else {
                //FIXME: Flush somewhere
                pause("Wrong choice! Only 'y' or 'n' are permitted.\nPress any key to continue...");
            }
        } while ((input != 'y') && (input != 'Y') && (input != 'n') && (input != 'N'));
    }
}

void patientAccountUi(char fiscalCode[]) {
    int userChoice = -1;
    bool running = true;
    Appointment appList = loadAppointmentList();
    // TODO: load Results list

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
                patientShowAppointmentUi(appList, fiscalCode);
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

void patientLoginUi(Patient ptList) {

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

        if (loginCheck(ptList, fiscalCode, password)) {
            //login authorized
            patientAccountUi(fiscalCode);
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

void patientUi() {
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
                patientLoginUi(ptList);
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

/* UI LATO LABORATORIO */

void labShowTestHistoryUi(int currentDay) {
    int userChoice = -1;
    TestResult rsList = loadTestResults();
    do {
        clearScreen();
        printf("Please make a choice:\n\n");
        printf("1. SHOW ALL TEST RESULTS\n");
        printf("2. SHOW TEST RESULTS WITH SPECIFIC DAY\n");
        printf("3. GO BACK\n");
        printf("\nYour choice: ");

        userChoice = getChoice(3);
    } while (userChoice == -1);

    switch (userChoice) {
        case 1:
            // By default, chronological order is used
            printTestResultList(rsList, true);
            pause("Press ENTER to go back...");
            break;
        case 2:
            userChoice = -1;
            do {
                clearScreen();
                printf("Choose a day (range: 1-%d): ", currentDay - 1);
                userChoice = getChoice(currentDay - 1);
            } while (userChoice == -1);
            clearScreen();
            printf("DAY %d:\n", userChoice);
            // By default, chronological order is used
            printTestResultsByDay(rsList, userChoice, true);
            pause("Press ENTER to go back...");
            break;
        case 3:
            break;
        default:
            break;
    }
}

void labLoginUi(TestReservation *test) {
    int userChoice = -1;
    bool running = true;
    int workerId;
    char password[PASSWORD_SIZE];
    LabWorker wkList = loadLabWorkers();
    do {
        clearScreen();
        printf("Please provide your worker ID: ");
        scanf("%d", &workerId); //FIXME: Controllo sull'input (hint: adattare getChoice?)
        fflush(stdin);
        printf("Please provide your password: ");
        scanf("%20s", password); //TODO: masked scanf
        fflush(stdin);
        if ( labLoginCheck(wkList, workerId, password) ) {
            //login authorized
            labUi(test);
            running = false;
        }
        else {
            do {
                //credentials are not correct
                //you can choose to stop and go back
                printf("Your credentials are incorrect.\n");
                printf("Please make a choice:\n");
                printf("1. TRY AGAIN\n");
                printf("2. GO BACK\n");
                printf("\nYour choice: ");
                userChoice = getChoice(2);
            } while (userChoice == -1);

            switch (userChoice) {
                // TODO: Delete if useless
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

void labUi(TestReservation *test) {
    int userChoice = -1;
    bool running = true;

    do {
        do {
            clearScreen();
            printf("Welcome to the COVID-19 testing centre - Lab Area.\n");
            printf("Please make a choice:\n\n");
            printf("1. SHOW CARRIED OUT TESTS HISTORY\n");
            printf("2. SHOW APPOINTMENT REQUESTS\n");
            printf("3. SHOW TEST RESERVATIONS\n");
            printf("4. ADD OR REMOVE A RESERVATION\n");
            printf("5. GO BACK TO THE MAIN MENU\n");
            printf("\nYour choice: ");

            userChoice = getChoice(5);
        } while (userChoice == -1);

        switch(userChoice) {
            case 1:
                labShowTestHistoryUi((*test)->currentDay);
                break;
            case 2:
                //labManageAppointmentRequestsUi();
                break;
            case 3:
                //labShowReservationsUi();
                break;
            case 4:
                //labManageReservationsUi();
                break;
            case 5:
                running = false;
                break;
            default:
                pause("Something went wrong...");
                // FIXME: Handle shutdown
                break;
        }
    } while (running);
}

/* UI PROGRAMMA */

void run() {
    //Bootstrap
    TestReservation test = newTestReservation();

    // Avvio programma
    mainUi(&test); //TODO: Deve essere propagato test per riferimento fino al laboratorio

    // Operazioni conclusive prima della chiusura del programma
    saveTestResultList(test);
    deleteTestReservation(test);
}

void mainUi(TestReservation *test) {
    int userChoice = -1;
    bool running = true;

    do {
        do {
            clearScreen();
            printf ("Welcome to the COVID-19 testing centre. Please make a choice:\n\n");
            printf ("1. ENTER PATIENT AREA;\n");
            printf ("2. ENTER LAB AREA;\n");
            printf ("3. EXIT THE PROGRAM\n");

            printf ("\nYour choice: ");

            userChoice = getChoice(3);
        } while (userChoice == -1);

        switch(userChoice) {
            case 1:
                patientUi();
                break;
            case 2:
                labLoginUi(test);
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
}

/* FUNZIONI AUSILIARIE */

void wrongChoice() {
    printf("Your choice is not correct! Please choose one of the numbers provided.\n");
    pause("Press any key to continue...");
}

int getChoice(int maxOptions) {
    int tmp, choice = -1;
    fflush(stdin);
    if ((scanf("%d", &tmp)) == 1) {
        if ((tmp > 0) && (tmp <= maxOptions)) {
            choice = tmp;
        }
        else wrongChoice();
    }
    else wrongChoice();
    return choice;
}


