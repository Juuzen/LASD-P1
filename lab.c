#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab.h"
#include "s_employee.h"
#include "s_result.h"
#include "database.h"
#include "helper.h"

/* FUNZIONI AUSILIARIE */

/* Restituisce true se il login va a buon fine */
bool labLoginCheck(Employee emList, int id, char password[]) {
    if (emList == NULL) return false;
    else {
        if ((emList->id == id) && (strcmp(emList->password, password) == 0)) return true;
        else return labLoginCheck(emList->next, id, password);
    }
}

/* Popola res sulla base degli appuntamenti contenuti in apList */
void labPopulateReservations(Reservation *res, Appointment apList) {
    if ((*res) == NULL) {
        (*res) = newReservation();
    }

    if (apList != NULL) {
        if (!isTimeSlotFull((*res), apList->slot)) {
            addReservation(res, appointmentCloneNode(apList));
        }

        labPopulateReservations(res, apList->next);
    }
}

/* Processa i vari appuntamenti ricevuti e li inserisce in Reservation */
void labConfirmAppointments(Reservation *res, Appointment *apList) {
    labPopulateReservations(res, (*apList));

    /* Una volta popolato res, quelli che non sono stati inclusi non lo saranno a prescindere per il giorno corrente
       e possono essere cancellati */
    appointmentFreeList((*apList));
    (*apList) = NULL;

    dropAppointmentDB();
}


/* FUNZIONI UI */

/* Funzione di UI per il punto E della traccia */
void labShowTestHistoryUi(int currentDay) {
    if (currentDay == 1) {
        clearScreen();
        printf("There are no test result up to now. You can choose this option from day 2 onward.\n");
        printMessage(PAUSE_DEFAULT);
    }
    else {
        int userChoice = -1;
        TestResult rsList = loadTestResultList();
        do {
            clearScreen();
            printf("Please make a choice:\n\n");
            printf("1. SHOW ALL TEST RESULTS\n");
            printf("2. SHOW TEST RESULTS WITH SPECIFIC DAY\n");
            printf("3. GO BACK\n");
            printf("\nYour choice: ");

            userChoice = getChoice(3);
        } while (userChoice == -1);
        printf ("\n");
        switch (userChoice) {
            case 1:
                /* Di default, vengono stampati i test in senso cronologico */
                testResultPrintList(rsList, true);
                printMessage(PAUSE_DEFAULT);
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

                TestResult filteredList = testResultFilterByDay(rsList, userChoice);
                /* Di default, vengono stampati i test in senso cronologico */
                testResultPrintList(filteredList, true);
                testResultFreeList(filteredList);
                printMessage(PAUSE_DEFAULT);
                break;

            default:
                break;
        }
        testResultFreeList(rsList);
    }
}

/* Funzione di UI per il punto F della traccia */
void labManageAppointmentRequestsUi(Reservation *res) {

    Appointment apList = loadAppointmentList();
    if (apList == NULL) {
        clearScreen();
        printf("There are no requests so far.\n");
        printMessage("Press ENTER key to go back...");
    }

    else {
        char input;
        do {
            clearScreen();
            printf("Here are the appointment request up to now:\n");
            appointmentPrintList(apList);
            printf("Do you want to start the confirmation process? [y/n] ");
            scanf("%c", &input);
            flushStdin();
            if ((input == 'y') || (input == 'Y')) {
                labConfirmAppointments(res, &apList);
                printf("Reservations made. You can check them by choosing option 3 in main menu.\n");
            }

            else if ((input == 'n') || (input == 'N')) {
                printf("The confirmation process is stopped. No changes have been made.\n");
                appointmentFreeList(apList);
            }

            else {
                printf("Wrong input: only 'y' or 'n' are permitted (case insensitive).\n");
                printMessage("Press ENTER key to try again...");
            }
        } while ((input != 'y') && (input != 'Y') && (input != 'n') && (input != 'N'));

        printMessage(PAUSE_DEFAULT);
    }
}

/* Funzione di UI per il pungo G della traccia */
void labShowReservationUi(Reservation res) {
    clearScreen();
    printf("Here are the test reservations for the current day.\n");
    printReservation(res);
    printMessage(PAUSE_DEFAULT);
}

/* Funzione di UI per aggiungere manualmente un appuntamento fissato */
void labAddReservationUi(Reservation *res) {
    int userChoice = -1;
    do {
        clearScreen();
        printf("Please choose a time slot:\n");
        printf("1. MORNING\n");
        printf("2. AFTERNOON\n");
        printf("3. EVENING\n");
        printf("4. GO BACK\n");

        userChoice = getChoice(4);
    } while (userChoice == -1);

    if (userChoice != 4) {
        timeSlot slot = (timeSlot) --userChoice;
        if (!isTimeSlotFull((*res), slot)) {
            char fiscalCode[FISCALCODE_SIZE];
            char *symptoms;

            printf("Please provide the fiscal code of the patient: ");
            scanf("%17s", fiscalCode);
            flushStdin();
            printf("If you have any symptoms, please provide a concise explanation (max %d characters);\n", SYMPTOMS_SIZE);
            printf("Otherwise just press ENTER KEY to go further.\n");
            printf("Your symptoms: ");
            symptoms = getSymptoms(stdin);

            Appointment manualApp = appointmentNewNode(fiscalCode, slot, symptoms);
            addReservation(res, manualApp);
            printf("Reservation created! Exit the program to process it.\n");
            printMessage("Press ENTER key to go back...");
        }

        else {
            printf("Reservations are at max capacity in this time slot.\n");
            printMessage("Press ENTER key to go back...");
        }
    }
}

/* Funzione di UI per rimuovere manualmente un appuntamento fissato */
void labRemoveReservationUi(Reservation *res) {
    int userChoice = -1;
    do {
        clearScreen();
        printf("Please choose a time slot:\n");
        printf("1. MORNING\n");
        printf("2. AFTERNOON\n");
        printf("3. EVENING\n");
        printf("4. GO BACK\n");

        userChoice = getChoice(4);
    } while (userChoice == -1);

    if (userChoice != 4) {
        timeSlot slot = (timeSlot) --userChoice;
        char fiscalCode[FISCALCODE_SIZE];
        clearScreen();
        switch (slot) {
            case MORNING:
                if ((*res)->morning == NULL) printf("There are no reservation for this time slot.\n");

                else {
                    printf("Reservation for the morning slot:\n");
                    appointmentPrintList((*res)->morning);
                    printf("\nPlease provide the fiscal code of the patient: ");
                    scanf("%17s", fiscalCode);
                    (*res)->morning = appointmentDeleteByFiscalCode((*res)->morning, fiscalCode);
                    printf("Reservation removed. You can check reservation by choosing the option 3 in the main menu.\n");
                }
                break;

            case AFTERNOON:
                if ((*res)->afternoon == NULL) printf("There are no reservation for this time slot.\n");

                else {
                    printf("Reservation for the afternoon slot:\n");
                    appointmentPrintList((*res)->afternoon);
                    printf("\nPlease provide the fiscal code of the patient: ");
                    scanf("%17s", fiscalCode);
                    (*res)->afternoon = appointmentDeleteByFiscalCode((*res)->afternoon, fiscalCode);
                    printf("Reservation removed. You can check reservation by choosing the option 3 in the main menu.\n");
                }
                break;

            case EVENING:
                if ((*res)->evening == NULL) printf("There are no reservation for this time slot.\n");
                else {
                    printf("Reservation for the evening slot:\n");
                    appointmentPrintList((*res)->evening);
                    printf("\nPlease provide the fiscal code of the patient: ");
                    scanf("%17s", fiscalCode);
                    (*res)->evening = appointmentDeleteByFiscalCode((*res)->evening, fiscalCode);
                    printf("Reservation removed. You can check reservation by choosing the option 3 in the main menu.\n");
                }
                break;

            default:
                break;
        }

        printMessage(PAUSE_DEFAULT);
    }
}

/* Funzione di UI per il punto H della traccia */
void labHandleReservationUi(Reservation *res) {
    int userChoice = -1;
    do {
        clearScreen();
        printf("Please make a choice:\n");
        printf("1. ADD A RESERVATION\n");
        printf("2. REMOVE A RESERVATION\n");
        printf("3. GO BACK\n");
        printf("Your choice: ");
        userChoice = getChoice(3);
    } while (userChoice == -1);

    if (userChoice != 3) {
        switch (userChoice) {
            case 1:
                labAddReservationUi(res);
                break;

            case 2:
                labRemoveReservationUi(res);
                break;

            default:
                break;
        }
    }
}

/* Funzione di UI per la pagina personale dell'impiegato */
void labMainMenuUi(Reservation *res) {
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
                labShowTestHistoryUi((*res)->currentDay);
                break;
            case 2:
                labManageAppointmentRequestsUi(res);
                break;
            case 3:
                labShowReservationUi((*res));
                break;
            case 4:
                labHandleReservationUi(res);
                break;
            case 5:
                running = false;
                break;
            default:
                break;
        }
    } while (running);
}

/* Funzione di UI per il login dell'impiegato */
void labLoginUi(Reservation *res) {
    int userChoice = -1;
    bool running = true;
    int id;
    char *password;
    Employee emList = loadEmployeeList();
    clearScreen();
    printf("For testing purposes, you can login with those credentials:\n");
    printf("ID: 1000\n");
    printf("Password: progetto\n");
    printMessage("Press ENTER key to continue...");
    do {
        clearScreen();
        id = getEmployeeId("Please provide your worker ID: ");
        printf("Please provide your password (the echoing is turned off): ");
        password = maskedInput();
        if ( labLoginCheck(emList, id, password) ) {
            /* Login autorizzato */
            labMainMenuUi(res);
            running = false;
        }
        else {
            do {
                /* Le credenziali non sono corrette */
                printf("Your credentials are incorrect.\n");
                printf("Please make a choice:\n");
                printf("1. TRY AGAIN\n");
                printf("2. GO BACK\n");
                printf("\nYour choice: ");
                userChoice = getChoice(2);
            } while (userChoice == -1);

            if (userChoice == 2) running = false;
        }
    } while (running);

    employeeFreeList(emList);
}

