#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab.h"
#include "structure.h"
#include "database.h"
#include "helper.h"

/* FUNZIONI AUSILIARIE */
bool labLoginCheck(LabWorker wkList, int workerId, char password[]) {
    if (wkList == NULL) return false;
    else {
        if ((wkList->id == workerId) && (strcmp(wkList->password, password) == 0)) return true;
        else return labLoginCheck(wkList->next, workerId, password);
    }
}
void labConfirmAppointments(TestReservation *reservation, Appointment *appList) {
    labPopulateReservations(reservation, (*appList));

    deleteAppointmentList((*appList));
    (*appList) = NULL;

    dropAppointmentDB();
}
void labPopulateReservations(TestReservation *reservation, Appointment appList) {
    if ((*reservation) == NULL) {
        (*reservation) = newTestReservation();
    }

    if (appList != NULL) {
        if (!isTimeSlotFull((*reservation), appList->slot)) {
            addReservation(reservation, cloneAppointment(appList));
        }

        labPopulateReservations(reservation, appList->next);
    }
}


/* FUNZIONI UI */
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
void labManageAppointmentRequestsUi(TestReservation *test) {

    Appointment appList = loadAppointmentList();
    if (appList == NULL) {
        clearScreen();
        printf("There are no requests so far.\n");
        pause("Press ENTER key to go back...");
    }

    else {
        char input;
        do {
            clearScreen();
            printf("Here are the appointment request up to now:\n");
            printAppointmentList(appList);
            printf("Do you want to start the confirmation process? [y/n] ");
            fflush(stdin);
            scanf("%c", &input);
            fflush(stdin);
            if ((input == 'y') || (input == 'Y')) {
                labConfirmAppointments(test, &appList);
                printf("Reservations made. You can check them by choosing option 3 in main menu.\n");
                pause("Press ENTER key to go back...");
            }

            else if ((input == 'n') || (input == 'N')) {
                printf("The confirmation process is stopped. No changes have been made.\n");
                pause("Press ENTER key to go back...");
            }

            else {
                printf("Wrong input: only 'y' or 'n' are permitted (case insensitive).\n");
                pause("Press ENTER key to try again...");
            }
        } while ((input != 'y') && (input != 'Y') && (input != 'n') && (input != 'N'));
    }
}
void labShowReservationUi(TestReservation test) {
    clearScreen();
    printf("Here are the test reservations for the current day.\n");
    printTestReservation(test);
    pause("Press ENTER key to go back...");
}
void labAddReservationUi(TestReservation *test) {
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
        if (!isTimeSlotFull((*test), slot)) {
            char fiscalCode[FISCALCODE_SIZE];
            char *symptoms;

            printf("Please provide the fiscal code of the patient: ");
            scanf("%17s", fiscalCode);

            printf("If you have any symptoms, please provide a concise explanation (max %d characters);\n", SYMPTOMS_SIZE);
            printf("Otherwise just press ENTER KEY to go further.\n");
            printf("Your symptoms: ");
            symptoms = getSymptoms(stdin);

            Appointment manualApp = newAppointmentNode(fiscalCode, slot, symptoms);
            addReservation(test, manualApp);
            printf("Reservation created! Exit the program to process it.\n");
            pause("Press ENTER key to go back...");
        }

        else {
            printf("Reservations are at max capacity in this time slot.\n");
            pause("Press ENTER key to go back...");
        }
    }
}
void labRemoveReservationUi(TestReservation *test) {
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
                printf("Reservation for the morning slot:\n");
                printAppointmentList((*test)->morning);
                break;
            case AFTERNOON:
                printf("Reservation for the afternoon slot:\n");
                printAppointmentList((*test)->afternoon);
                break;
            case EVENING:
                printf("Reservation for the evening slot:\n");
                printAppointmentList((*test)->evening);
                break;
            default:
                break;
        }

        printf("\nPlease provide the fiscal code of the patient: ");
        scanf("%17s", fiscalCode);

        switch (slot) {
            case MORNING:
                (*test)->morning = deleteAppointmentByFiscalCode((*test)->morning, fiscalCode);
                break;
            case AFTERNOON:
                (*test)->afternoon = deleteAppointmentByFiscalCode((*test)->afternoon, fiscalCode);
                break;
            case EVENING:
                (*test)->evening = deleteAppointmentByFiscalCode((*test)->evening, fiscalCode);
                break;
            default:
                break;
        }

        printf("Reservation removed. You can check reservation by choosing the option 3 in the main menu.\n");
        pause("Press ENTER key to go back...");
    }
}
void labHandleReservationUi(TestReservation *test) {
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
                labAddReservationUi(test);
                break;

            case 2:
                labRemoveReservationUi(test);
                break;

            default:
                break;
        }
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
            labMainMenuUi(test);
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
void labMainMenuUi(TestReservation *test) {
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
                labManageAppointmentRequestsUi(test);
                break;
            case 3:
                labShowReservationUi((*test));
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
