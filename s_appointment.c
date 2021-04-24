#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s_appointment.h"
#include "helper.h"

/* Restituisce un puntatore Appointment = NULL */
Appointment appointmentNewList() {
    Appointment apList = NULL;
    return apList;
}

/* Restituisce, se possibile, un elemento Appointment con i valori passati in ingresso */
/* Altrimenti, restituisce NULL */
Appointment appointmentNewNode(char fiscalCode[], timeSlot slot, char symptoms[]) {
    Appointment apNode = (Appointment) calloc(1, sizeof(struct appointment));

    if (apNode != NULL) {
        strcpy(apNode->fiscalCode, fiscalCode);
        if (symptoms != NULL) {
            apNode->symptoms = (char *) calloc(1, SYMPTOMS_SIZE * sizeof(char));
            strcpy(apNode->symptoms, symptoms);
        }
        apNode->slot = slot;
    }
    return apNode;
}

/* Restituisce, se possibile, un nuovo elemento Appointment con gli stessi valori dell'elemento passato in ingresso */
/* Altrimenti, restituisce NULL */
Appointment appointmentCloneNode(Appointment apNode) {
    if (apNode == NULL) return NULL;
    else return appointmentNewNode(apNode->fiscalCode, apNode->slot, apNode->symptoms);
}

/* Dealloca un elemento Appointment, pulendo anche il suo contenuto */
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

/* Dealloca una intera lista di elementi Appointment */
void appointmentFreeList (Appointment apList) {
    if (apList != NULL) {
        appointmentFreeList(apList->next);
        appointmentFreeNode(apList);
    }
}

/* Inserisce un elemento Appointment in coda ad apList */
Appointment appointmentAppendToList (Appointment apList, Appointment appointment) {
    if (apList == NULL) return appointment;

    else {
        apList->next = appointmentAppendToList (apList->next, appointment);
        return apList;
    }
}

/* Alloca un elemento Appointment e lo inserisce in coda ad apList */
Appointment appointmentTailInsert(Appointment apList, char fiscalCode[], timeSlot slot, char symptoms[]) {
    if (apList == NULL) return appointmentNewNode(fiscalCode, slot, symptoms);

    else {
        apList->next = appointmentTailInsert(apList->next, fiscalCode, slot, symptoms);
        return apList;
    }
}

/* Stampa su stdout un elemento Appointment */
void appointmentPrintNode(Appointment apNode) {
    if (apNode != NULL) {
        printf("Fiscal Code: %s\n", apNode->fiscalCode);
        printf("Time slot: %s\n", getTimeSlot(apNode->slot));
        printf("Symptoms: ");
        if (apNode->symptoms == NULL) printf("///");
        else printf("%s", apNode->symptoms);
        printf("\n\n");
    }
}

/* Stampa su stdout una lista di elementi Appointment */
void appointmentPrintList(Appointment apList) {
    if (apList != NULL) {
        appointmentPrintNode(apList);
        appointmentPrintList(apList->next);
    }
}

/* Restituisce, se presente, un elemento Appointment il cui campo fiscalCode corrisponde al valore in ingresso */
Appointment appointmentFindByFiscalCode(Appointment apList, char fiscalCode[]) {
    if (apList == NULL) return NULL;
    else {
        if (strcmp(apList->fiscalCode, fiscalCode) == 0) return apList;
        else return appointmentFindByFiscalCode(apList->next, fiscalCode);
    }
}

/* Restituisce una lista di elementi Appointment priva di tutti gli elementi il cui campo fiscalCode */
/* Corrisponde al valore passato in ingresso */
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

/* Restituisce il numero di elementi Appointment presenti nella lista apList */
int appointmentListCount(Appointment apList) {
    if (apList == NULL) return 0;
    else return 1 + appointmentListCount(apList->next);
}
