#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "project.h"

#include "patient.h"
#include "lab.h"
#include "helper.h"
#include "database.h"

void projectRun() {
    //Bootstrap
    srand(time(NULL));
    Reservation res = newReservation();
    Quarantine qtList = loadQuarantineList();

    // Avvio programma
    mainUi(&res, qtList);

    // Operazioni conclusive prima della chiusura del programma
    saveTestResultList(res, &qtList);
    saveQuarantineList(qtList);
    freeReservation(res);
    quarantineFreeList(qtList);
}

void mainUi(Reservation *res, Quarantine qtList) {
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
                patientMainMenuUi(res, qtList);
                break;
            case 2:
                labLoginUi(res);
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


