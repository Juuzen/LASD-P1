#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s_appointment.h"

Appointment appointmentNewList() {
    Appointment apList = NULL;
    return apList;
}

Appointment appointmentNewNode(char fiscalCode[], timeSlot slot, char symptoms[]) {
    Appointment apNode = (Appointment) calloc(1, sizeof(struct appointment));

    if (apNode != NULL) {
        strcpy(apNode->fiscalCode, fiscalCode);
        // TODO: Controllare come viene generato symptoms
        if (symptoms != NULL) {
            apNode->symptoms = (char *) calloc(1, SYMPTOMS_SIZE * sizeof(char));
            strcpy(apNode->symptoms, symptoms);
        }
        apNode->slot = slot;
    }
    return apNode;
}

Appointment appointmentCloneNode(Appointment apNode) {
    if (apNode == NULL) return NULL;
    else return appointmentNewNode(apNode->fiscalCode, apNode->slot, apNode->symptoms);
}

void appointmentFreeNode (Appointment apNode) {
    if (apNode != NULL) {
        memset(apNode->fiscalCode, '\0', sizeof(apNode->fiscalCode));
        if (apNode->symptoms != NULL) {
            free(apNode->symptoms);
            apNode->symptoms = NULL;
        }
        apNode->slot = 0;
        free(apNode);
    }
}

void appointmentFreeList (Appointment apList) {
    if (apList != NULL) {
        appointmentFreeList(apList->next);
        appointmentFreeNode(apList);
    }
}

Appointment appointmentAppendToList (Appointment apList, Appointment appointment) {
    if (apList == NULL) return appointment;

    else {
        apList->next = appointmentAppendToList (apList->next, appointment);
        return apList;
    }
}

Appointment appointmentTailInsert(Appointment apList, char fiscalCode[], timeSlot slot, char symptoms[]) {
    if (apList == NULL) return appointmentNewNode(fiscalCode, slot, symptoms);

    else {
        apList->next = appointmentTailInsert(apList->next, fiscalCode, slot, symptoms);
        return apList;
    }
}

void appointmentPrintNode(Appointment apNode) {
    if (apNode != NULL) {
        printf("Fiscal Code: %s\n", apNode->fiscalCode);
        printf("Time slot: %s\n", getTimeSlot(apNode->slot));
        printf("Symptoms: ");
        printf((apNode->symptoms == NULL) ? "///" : apNode->symptoms);
        printf("\n\n");
    }
}

void appointmentPrintList(Appointment apList) {
    if (apList != NULL) {
        appointmentPrintNode(apList);
        appointmentPrintList(apList->next);
    }
}

Appointment appointmentFindByFiscalCode(Appointment apList, char fiscalCode[]) {
    if (apList == NULL) return NULL;
    else {
        if (strcmp(apList->fiscalCode, fiscalCode) == 0) return apList;
        else return appointmentFindByFiscalCode(apList->next, fiscalCode);
    }
}

Appointment appointmentDeleteByFiscalCode(Appointment apList, char fiscalCode[]) {
    if (apList == NULL) return apList;
    else {
        if (strcmp(apList->fiscalCode, fiscalCode) == 0) {
            Appointment tmp = apList->next;
            appointmentFreeNode(apList);
            return tmp;
        }
        else {
            apList->next = appointmentDeleteByFiscalCode(apList->next, fiscalCode);
            return apList;
        }
    }
}

int appointmentListCount(Appointment appList) {
    if (appList == NULL) return 0;
    else return 1 + appointmentListCount(appList->next);
}
