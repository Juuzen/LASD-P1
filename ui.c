#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ui.h"
#include "structure.h"
#include "patient.h"
#include "helper.h"
#include "database.h"

void patientAccountUi(Patient ptList, char fiscalCode[]) {
    int userChoice = -1;
    bool running = true;
    // TODO: load Appointment queue
    // TODO: load Results list
    do {
        do {
            clearScreen();
            printf("Welcome %s.\n", fiscalCode);
            printf("Please make a choice:\n\n");
            printf("1. REQUEST A COVID TEST APPOINTMENT\n");
            printf("2. CHECK YOUR APPOINTMENT\n");
            printf("3. CANCEL AN APPOINTMENT\n");
            printf("4. SHOW THE RESULTS FOR PREVIOUS TESTS\n");
            printf("5. GO BACK\n");
            printf("\nYour choice: ");

            userChoice = getChoice(3);
        } while (userChoice == -1);

        switch(userChoice) {
            case 1:

                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

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

    savePatientList(ptList);
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
        printf("Please choose a password: ");
        //TODO: masked scanf
        scanf("%20s", password);
        fflush(stdin);

        if (loginCheck(ptList, fiscalCode, password)) {
            //login authorized
            patientAccountUi(ptList, fiscalCode);
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
                //patientLoginUi();
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

    savePatientList(ptList);
}

void labUi() {
    clearScreen();

}

void mainUi() {
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
                //labUi();
                printf("I'm the lab.\n");
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

void wrongChoice() {
    printf("Your choice is not correct! Please choose one of the numbers provided.\n");
    printf("Press any key to continue... ");
    fflush(stdin);
    getchar();
    fflush(stdin);
}

int getChoice(int maxOptions) {
    int tmp, choice = -1;
    if ((scanf("%d", &tmp)) == 1) {
        if ((tmp > 0) && (tmp <= maxOptions)) {
            choice = tmp;
        }
        else wrongChoice();
    }
    else wrongChoice();
    return choice;
}

void run() {
    // TODO: Tutte le funzioni di boot

    mainUi();

    // TODO: Tutte le funzioni di chiusura
}
