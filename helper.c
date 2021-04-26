#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#ifdef _WIN32
#include <conio.h>
#endif

#ifdef __linux__
#include <termios.h>
#include <unistd.h>
#endif

#include "const.h"
#include "helper.h"
#include "s_employee.h"

/* Restituisce una lista pregenerata di elementi Employee */
Employee loadSampleEmployeeList() {
    Employee emList = employeeNewList();
    emList = employeeTailInsert(emList, 1000, "progetto");
    return emList;
}

/* Ripulisce lo schermo, permettendo una pi� facile visualizzazione delle informazioni */
void clearScreen() {
    #ifdef _WIN32
    /* Istruzioni per sistema operativo Windows */
    system("cls");
    #endif

    #ifdef __linux__
    /* Istruzioni per sistema operativo Linux */
    system("clear");
    #endif
}

/* Prende in input una stringa, mascherando l'output corrispettivo di ci� che � stato immesso */
char* maskedInput() {
    char* password = (char *) calloc(1, PASSWORD_SIZE * sizeof(char));
    #ifdef _WIN32
    /* Istruzioni per sistema operativo Windows */
    char c;

    int i;
    for (i = 0; i < PASSWORD_SIZE - 1; i++) {
        /* Prende un carattere da stdout senza buffer */
        c = getch();
        /* 13 = carriage return */
        if (c == 13) break;
        else password[i] = c;
    }
    password[i] = '\0';
    printf("\n");
    #endif

    #ifdef __linux__
    /* Istruzioni per sistema operativo Linux */
    struct termios tp, save;
    /* Se possibile, salva i dati relativi al terminale in tp*/
    if (tcgetattr(STDIN_FILENO, &tp) != -1) {
        /* Salva le precedenti impostazioni */
        save = tp;
        /* Imposta il flag ECHO a 0 (nessun input verr� propagato su stdout) */
        tp.c_lflag &= ~ECHO;
        /* Se possibile, salva le nuove impostazioni */
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) != -1) {
            /* Effettua la lettura da input */
            scanf("%20s", password);
            flushStdin();
            /* Ripristina le precedenti impostazioni */
            tcsetattr(STDIN_FILENO, TCSANOW, &save);
        }
    }
    #endif

    return password;
}

/* Stampa su stdout un messaggio passato in input, mettendo in pausa il proseguimento dell'applicativo */
/* Per procedere basta premere il tasto INVIO */
void printMessage(char message[]) {
    printf("%s\n", message);
    flushStdin();
}

/* Funzione fittizia per generare gli esiti dei test COVID-19 */
bool generateTestResult() {
    return rand() % 2;
}

/* Riceve da input un numero intero, assicurandosene la correttezza (eg: no caratteri) */
/* In caso negativo, stampa un messaggio su stdout e ripete */
int getEmployeeId(char message[]) {
    bool correct = false;
    int choice;

    do {
        clearScreen();
        printf("%s", message);
        if (scanf("%d", &choice) == 1) {
            flushStdin();
            correct = true;
        }
        else {
            flushStdin();
            printf("Only numbers are permitted.\n");
            printMessage("Press ENTER key to try again...");
        }
    } while (!correct);
    return choice;
}

/* Ritorna il giorno corrente, ricavando quello precedente leggendo da file */
/* Se il file non esiste ancora, restituisce il giorno 1 */
int getCurrentDay() {
    int scan, lastDay = 0;
    char c;
    FILE *file = fopen(TESTRESULT_DB, "r");
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        if (ftell(file) == 0){
            //FILE VUOTO
        }
        else {
            fseek(file, -3, SEEK_CUR);
            do {
                scan = fgetc(file);
                c = (char) scan;
                if (c != '\t') {
                    fseek(file, -2, SEEK_CUR);
                }
            } while (c != '\t');

            fscanf(file, "%d", &lastDay);
        }
        fclose(file);
    }

    else {
        // FILE NON ESISTE
        file = fopen(TESTRESULT_DB, "w");
        fclose(file);
    }
    return lastDay + 1;
}

/* Ritorna una stringa sulla base della fascia oraria ricevuta in ingresso */
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

/* Ritorna una stringa letta da input, comprensiva di whitespace e altri caratteri speciali */
/* L'eccezione � '\n' */
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
    printMessage("Press any key to continue...");
}

/* Ritorna un intero letto da input, corretto sia nel tipo che nel range (0 - maxOptions) */
int getChoice(int maxOptions) {
    int tmp, choice = -1;
    if ((scanf("%d", &tmp)) == 1) {
        flushStdin();
        if ((tmp > 0) && (tmp <= maxOptions)) {
            choice = tmp;
        }
        else wrongChoice();
    }
    else {
        flushStdin();
        wrongChoice();
    }
    return choice;
}

void flushStdin() {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}