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
            // In questa maniera viene stampato in maniera anti-cronologica (pi� recente -> meno recente)
            testResultPrintList(rsList->next, chronoOrder);
        }

        testResultPrintNode(rsList);

        if (chronoOrder) {
            // In questa maniera viene stampato in maniera cronologica (meno recente -> pi� recente)
            testResultPrintList(rsList->next, chronoOrder);
        }

    }
}

/* Restituisce una lista di elementi TestResult filtrata sulla base del parametro in ingresso */
TestResult testResultFilterByDay (TestResult rsList, int day) {
    if (rsList == NULL) return NULL;
    else {
        if (rsList->day == day) {
            TestResult node = testResultNewNode(rsList->fiscalCode, rsList->response, rsList->day);
            node->next = testResultFilterByDay(rsList->next, day);
            return node;
        }
        else return testResultFilterByDay(rsList->next, day);
    }
}

/* Restituisce una lista di elementi TestResult filtrata sulla base del parametro in ingresso */
TestResult testResultFilterByFiscalCode (TestResult rsList, char fiscalCode[]) {
    if (rsList == NULL) return NULL;
    else {
        if (strcmp(rsList->fiscalCode, fiscalCode) == 0) {
            TestResult node = testResultNewNode(rsList->fiscalCode, rsList->response, rsList->day);
            node->next = testResultFilterByFiscalCode(rsList->next, fiscalCode);
            return node;
        }
        else return testResultFilterByFiscalCode(rsList->next, fiscalCode);
    }
}
