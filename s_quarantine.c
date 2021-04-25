#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "s_quarantine.h"


Quarantine quarantineNewList() {
    Quarantine list = NULL;
    return list;
}

Quarantine quarantineNewNode (char fiscalCode[]) {
    Quarantine node = (Quarantine) calloc(1, sizeof(struct quarantine));
    if(node != NULL) {
        strcpy (node->fiscalCode, fiscalCode);
        node->next = NULL;
    }
    return node;
}

void quarantineFreeNode (Quarantine node) {
    if (node != NULL) {
        memset (node->fiscalCode, '\0', sizeof(node->fiscalCode));
        node->next = NULL;
        free (node);
    }
}

void quarantineFreeList (Quarantine qtList) {
    if (qtList != NULL) {
        quarantineFreeList (qtList->next);
        quarantineFreeNode (qtList);
    }
}

Quarantine quarantineTailInsert (Quarantine qtList, char fiscalCode[]) {
    if (qtList == NULL) return quarantineNewNode (fiscalCode);
    else {
        /* Per evitare duplicazioni */
        if (strcmp(qtList->fiscalCode, fiscalCode) != 0) {
            qtList->next = quarantineTailInsert (qtList->next, fiscalCode);
        }
        return qtList;
    }
}

void quarantinePrintNode(Quarantine node) {
    if (node != NULL) {
        printf("The patient with fiscal code %s is in QUARANTINE.\n", node->fiscalCode);
    }
}

void quarantinePrintList(Quarantine qtList) {
    if (qtList != NULL) {
        quarantinePrintNode (qtList);
        quarantinePrintList (qtList->next);
    }
}

Quarantine quarantineDeleteNodeByFiscalCode(Quarantine qtList, char fiscalCode[]) {
    if (qtList == NULL) return qtList;
    else {
        if (strcmp(qtList->fiscalCode, fiscalCode) == 0) {
            Quarantine tmp = qtList->next;
            quarantineFreeNode(qtList);
            return tmp;
        }
        else {
            qtList->next = quarantineDeleteNodeByFiscalCode(qtList->next, fiscalCode);
            return qtList;
        }
    }
}

bool quarantineFindPatientByFiscalCode(Quarantine qtList, char fiscalCode[]) {
    bool response = false;
    if (qtList != NULL) {
        if (strcmp(qtList->fiscalCode, fiscalCode) == 0) response = true;
        else response = quarantineFindPatientByFiscalCode(qtList->next, fiscalCode);
    }
    return response;
}

