#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "const.h"
#include "helper.h"

void clearScreen() {
    #ifdef OS_WINDOWS
    system("cls");
    #endif

    #ifdef OS_LINUX
    system("clear");
    #endif

    #ifdef OS_APPLE
    system("clear");
    #endif
}

void pause(char message[]) {
    printf("%s\n", message);
    fflush(stdin);
    getchar(); //FIXME: Qualsiasi tasto premuto (ora accetta solo invio)
    fflush(stdin);
}

bool generateTestResult() {
    srand(time(NULL));
    return rand() % 2;
}

int getCurrentDay() {
    FILE * fp = fopen(TESTRESULT_DB, "r");

    if (fp != NULL) {
        fseek(fp, -1, SEEK_END);
        char c;
        do {
            fseek(fp, -2, SEEK_CUR);
            c = fgetc(fp);

        } while (c != '\t');

        int lastDay;
        if (fscanf(fp, "%d", &lastDay) > 0) {
            lastDay += 1;
        }
        else {
            //TODO: Gestire errore in lettura del giorno
        }
        fclose(fp);
        return lastDay;
    }
    else {
        //FIXME: Gestire fopen quando il file non esiste (crearlo? restituire 1?)
        return 1;
    }
}

const char* getTimeSlot(timeSlot slot) {
    switch (slot) {
        case MORNING:
            return "MORNING";
        case AFTERNOON:
            return "AFTERNOON";
        case EVENING:
            return "EVENING";
        default:
            return "";
    }
}

char * getSymptoms(FILE * file) {
    if (file != NULL) {
        int i = 0;
        char c;
        char * scan = (char *) calloc(1, SYMPTOMS_SIZE * sizeof(char));
        while (((c = fgetc(file)) != EOF) && c != '\n' && i < SYMPTOMS_SIZE) {
            scan[i] = c;
            i++;
        }

        if (feof(file)) {
            printf("FILE FINITO\n");
            return NULL;
        }
        return scan;
    }
    else return NULL;
}

void wrongChoice() {
    printf("Your choice is not correct! Please choose one of the numbers provided.\n");
    pause("Press any key to continue...");
}

int getChoice(int maxOptions) {
    int tmp, choice = -1;
    fflush(stdin);
    if ((scanf("%d", &tmp)) == 1) {
        fflush(stdin);
        if ((tmp > 0) && (tmp <= maxOptions)) {
            choice = tmp;
        }
        else wrongChoice();
    }
    else {
        fflush(stdin);
        wrongChoice();
    }
    return choice;
}
