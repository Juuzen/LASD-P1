#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void quarantineDeleteNode (Quarantine node) {
    if (node != NULL) {
        memset (node->fiscalCode, '\0', sizeof(node->fiscalCode));
        node->next = NULL;
        free (node);
    }
}

void quarantineDeleteList (Quarantine qtList) {
    if (qtList != NULL) {
        quarantineDeleteList (qtList->next);
        quarantineDeleteNode (qtList);
    }
}

Quarantine quarantineTailInsert (Quarantine qtList, char fiscalCode[]) {
    if (qtList == NULL) return quarantineNewNode (fiscalCode);
    else {
        qtList->next = quarantineTailInsert (qtList->next, fiscalCode);
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
