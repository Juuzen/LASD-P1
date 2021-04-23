#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s_result.h"

/* Restituisce un puntatore TestResult = NULL */
TestResult testResultNewList() {
    TestResult rsList = NULL;
    return rsList;
}

/* Restituisce, se possibile, un elemento TestResult con i valori passati in ingresso */
/* Altrimenti, restituisce NULL */
TestResult testResultNewNode(char fiscalCode[], char response[], int day) {
    TestResult rsNode = (TestResult) calloc(1, sizeof(struct testResult));
    if (rsNode != NULL) {
        strcpy(rsNode->fiscalCode, fiscalCode);
        strcpy(rsNode->response, response);
        rsNode->day = day;
        rsNode->next = NULL;
    }

    return rsNode;
}

/* Dealloca un elemento TestResult, pulendo anche il suo contenuto */
void testResultFreeNode(TestResult rsNode) {
    if (rsNode != NULL) {
        memset(rsNode->fiscalCode, '\0', sizeof(rsNode->fiscalCode));
        memset(rsNode->response, '\0', sizeof(rsNode->response));
        rsNode->day = 0;

        free(rsNode);
    }
}

/* Dealloca una intera lista di elementi TestResult */
void testResultFreeList(TestResult rsList) {
    if (rsList != NULL) {
        testResultFreeList(rsList->next);
        testResultFreeNode(rsList);
    }
}

/* Alloca un elemento TestResult e lo inserisce in coda a rsList */
TestResult testResultTailInsert(TestResult rsList, char fiscalCode[], char response[], int day) {
    if (rsList == NULL) return testResultNewNode(fiscalCode, response, day);
    else {
        rsList->next = testResultTailInsert(rsList->next, fiscalCode, response, day);
        return rsList;
    }
}

/* Stampa su stdout un elemento TestResult */
void testResultPrintNode(TestResult rsNode) {
    if (rsNode != NULL) {
        printf("DAY: %d - %s: %s\n", rsNode->day, rsNode->fiscalCode, rsNode->response);
    }
}

/* Stampa su stdout una lista di elementi TestResult, in maniera (anti)cronologica sulla base di chronoOrder */
void testResultPrintList(TestResult rsList, bool chronoOrder) {
    if (rsList != NULL) {
        if (!chronoOrder) {
            // In questa maniera viene stampato in maniera anti-cronologica (più recente -> meno recente)
            testResultPrintList(rsList->next, chronoOrder);
        }

        testResultPrintNode(rsList);

        if (chronoOrder) {
            // In questa maniera viene stampato in maniera cronologica (meno recente -> più recente)
            testResultPrintList(rsList->next, chronoOrder);
        }

    }
}

/* Stampa su stdout una lista di elementi TestResult, in maniera (anti)cronologica sulla base di chronoOrder*/
/* Stampa solo gli elementi il cui campo day corrisponde al valore in ingresso */
void testResultPrintByDay (TestResult rsList, int day, bool chronoOrder) {
    if (rsList != NULL) {
        if (!chronoOrder) {
            // In questa maniera viene stampato in maniera anti-cronologica (più recente -> meno recente)
            testResultPrintByDay(rsList->next, day, chronoOrder);
        }

        if (rsList->day == day) {
            printf("Patient: %s - %s\n", rsList->fiscalCode, rsList->response);
        }

        if (chronoOrder) {
            // In questa maniera viene stampato in maniera cronologica (meno recente -> più recente)
            testResultPrintByDay(rsList->next, day, chronoOrder);
        }
    }
}

/* Stampa su stdout una lista di elementi TestResult, in maniera (anti)cronologica sulla base di chronoOrder*/
/* Stampa solo gli elementi il cui campo fiscalCode corrisponde al valore in ingresso */
void testResultPrintByFiscalCode (TestResult rsList, char fiscalCode[], bool chronoOrder) {
    if (rsList != NULL) {
        if (!chronoOrder) {
            // In questa maniera viene stampato in maniera anti-cronologica (più recente -> meno recente)
            testResultPrintByFiscalCode(rsList->next, fiscalCode, chronoOrder);
        }

        if ((strcmp(rsList->fiscalCode, fiscalCode)) == 0) {
            printf("DAY %d - %s\n", rsList->day, rsList->response);
        }

        if (chronoOrder) {
            // In questa maniera viene stampato in maniera cronologica (meno recente -> più recente)
            testResultPrintByFiscalCode(rsList->next, fiscalCode, chronoOrder);
        }

    }
}

