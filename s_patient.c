#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s_patient.h"
#include "const.h"

Patient patientNewList() {
    Patient ptList = NULL;
    return ptList;
}

Patient patientNewNode(char fiscalCode[], char password[]) {
    Patient ptNode = (Patient) calloc(1, sizeof(patient));
    if (ptNode != NULL) {
        strncpy(ptNode->fiscalCode, fiscalCode, FISCALCODE_SIZE);
        strncpy(ptNode->password, password, PASSWORD_SIZE);
        ptNode->next = NULL;
    }
    return ptNode;
}

Patient patientTailInsert(Patient ptList, char fiscalCode[], char password[]) {
    if (ptList == NULL) return patientNewNode(fiscalCode, password);
    else {
        ptList->next = patientTailInsert(ptList->next, fiscalCode, password);
        return ptList;
    }
}

void patientFreeNode(Patient ptNode) {
    if (ptNode != NULL) {
        memset(ptNode->fiscalCode, '\0', sizeof(ptNode->fiscalCode));
        memset(ptNode->password, '\0', sizeof(ptNode->password));
        ptNode->next = NULL;
        free(ptNode);
    }
}

void patientFreeList(Patient ptList) {
    if (ptList != NULL) {
        patientFreeList(ptList->next);
        patientFreeNode(ptList);
    }
}

void patientPrintNode (Patient ptNode) {
    if (ptNode != NULL) {
        printf("%s | %s\n", ptNode->fiscalCode, ptNode->password);
    }
}

void patientPrintList(Patient ptList) {
    if (ptList != NULL) {
        patientPrintNode(ptList);
        patientPrintList(ptList->next);
    }
}