#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab.h"
#include "s_employee.h"
#include "structure.h"
#include "database.h"
#include "helper.h"

/* FUNZIONI AUSILIARIE */
bool labLoginCheck(Employee emList, int id, char password[]) {
    if (emList == NULL) return false;
    else {
        if ((emList->id == id) && (strcmp(emList->password, password) == 0)) return true;
        else return labLoginCheck(emList->next, id, password);
    }
}

void labConfirmAppointments(Reservation *res, Appointment *apList) {
    labPopulateReservations(res, (*apList));

    appointmentFreeList((*apList));
    (*apList) = NULL;

    dropAppointmentDB();
}

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

void labManageAppointmentRequestsUi(Reservation *res) {

    Appointment apList = loadAppointmentList();
    if (apList == NULL) {
        clearScreen();
        printf("There are no requests so far.\n");
        pause("Press ENTER key to go back...");
    }

    else {
        char input;
        do {
            clearScreen();
            printf("Here are the appointment request up to now:\n");
            appointmentPrintList(apList);
            printf("Do you want to start the confirmation process? [y/n] ");
            fflush(stdin);
            scanf("%c", &input);
            fflush(stdin);
            if ((input == 'y') || (input == 'Y')) {
                labConfirmAppointments(res, &apList);
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
void labShowReservationUi(Reservation res) {
    clearScreen();
    printf("Here are the test reservations for the current day.\n");
    printReservation(res);
    pause("Press ENTER key to go back...");
}
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

            printf("If you have any symptoms, please provide a concise explanation (max %d characters);\n", SYMPTOMS_SIZE);
            printf("Otherwise just press ENTER KEY to go further.\n");
            printf("Your symptoms: ");
            symptoms = getSymptoms(stdin);

            Appointment manualApp = appointmentNewNode(fiscalCode, slot, symptoms);
            addReservation(res, manualApp);
            printf("Reservation created! Exit the program to process it.\n");
            pause("Press ENTER key to go back...");
        }

        else {
            printf("Reservations are at max capacity in this time slot.\n");
            pause("Press ENTER key to go back...");
        }
    }
}
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
                printf("Reservation for the morning slot:\n");
                appointmentPrintList((*res)->morning);
                break;
            case AFTERNOON:
                printf("Reservation for the afternoon slot:\n");
                appointmentPrintList((*res)->afternoon);
                break;
            case EVENING:
                printf("Reservation for the evening slot:\n");
                appointmentPrintList((*res)->evening);
                break;
            default:
                break;
        }

        printf("\nPlease provide the fiscal code of the patient: ");
        scanf("%17s", fiscalCode);

        switch (slot) {
            case MORNING:
                (*res)->morning = appointmentDeleteByFiscalCode((*res)->morning, fiscalCode);
                break;
            case AFTERNOON:
                (*res)->afternoon = appointmentDeleteByFiscalCode((*res)->afternoon, fiscalCode);
                break;
            case EVENING:
                (*res)->evening = appointmentDeleteByFiscalCode((*res)->evening, fiscalCode);
                break;
            default:
                break;
        }

        printf("Reservation removed. You can check reservation by choosing the option 3 in the main menu.\n");
        pause("Press ENTER key to go back...");
    }
}

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

void labLoginUi(Reservation *res) {
    int userChoice = -1;
    bool running = true;
    int id;
    char password[PASSWORD_SIZE];
    Employee emList = loadEmployeeList();
    do {
        clearScreen();
        printf("Please provide your worker ID: ");
        scanf("%d", &id); //FIXME: Controllo sull'input (hint: adattare getChoice?)
        fflush(stdin);
        printf("Please provide your password: ");
        scanf("%20s", password); //TODO: masked scanf
        fflush(stdin);
        if ( labLoginCheck(emList, id, password) ) {
            //login authorized
            labMainMenuUi(res);
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
    employeeFreeList(emList);
}
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
