#include <stdio.h>
#include <stdlib.h>
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
    getchar();
    fflush(stdin);
}
