#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "s_reservation.h"
#include "s_appointment.h"
#include "helper.h"

/* Restituisce, se possibile, un elemento Reservation con l'elemento currentDay settato al giorno corrente */
/* Altrimenti, restituisce NULL */
Reservation newReservation() {
    Reservation res = (Reservation) calloc(1, sizeof(struct reservation));
    res->morning = NULL;
    res->afternoon = NULL;
    res->evening = NULL;
    res->currentDay = getCurrentDay();
    return res;
}

/* Dealloca un elemento Reservation, pulendo anche il suo contenuto */
void freeReservation(Reservation res) {
    if (res != NULL) {
        appointmentFreeList(res->morning);
        appointmentFreeList(res->afternoon);
        appointmentFreeList(res->evening);
        res->morning = NULL;
        res->afternoon = NULL;
        res->evening = NULL;
        res->currentDay = 0;
        free(res);
    }
}

/* Stampa su stdout un elemento Reservation */
void printReservation(Reservation res) {
    if (res != NULL) {
        printf("DAY %d:\n", res->currentDay);
        printf("Morning tests:\n");
        if (res->morning == NULL) printf("No reservations.\n");
        else appointmentPrintList(res->morning);

        printf("\nAfternoon tests:\n");
        if (res->afternoon == NULL) printf("No reservations.\n");
        else appointmentPrintList(res->afternoon);

        printf("\nEvening tests:\n");
        if (res->evening == NULL) printf("No reservations.\n\n");
        else appointmentPrintList(res->evening);
    }
}

/* Restituisce true se l'elemento Reservation non ha più posto nella fascia oraria passata in ingresso */
/* Altrimenti restituisce false */
bool isTimeSlotFull(Reservation res, timeSlot slot) {
    bool response = true;
    if (res != NULL) {
        switch (slot) {
            case MORNING:
                if (appointmentListCount(res->morning) < SLOT_CAPABILITY) {
                    response = false;
                }
                break;
            case AFTERNOON:
                if (appointmentListCount(res->afternoon) < SLOT_CAPABILITY) {
                    response = false;
                }
                break;
            case EVENING:
                if (appointmentListCount(res->evening) < SLOT_CAPABILITY) {
                    response = false;
                }
                break;
            default:
                break;
        }
    }

    return response;
}

/* Restituisce, se presente, l'elemento Appointment contenuto in res, che corrisponde al codice fiscale in ingresso */
/* Altrimenti restituisce NULL */
Appointment searchReservationByFiscalCode(Reservation res, char fiscalCode[]) {
    Appointment app = NULL;
    if (res != NULL) {
        app = appointmentFindByFiscalCode(res->morning, fiscalCode);
        if (app == NULL) {
            app = appointmentFindByFiscalCode(res->afternoon, fiscalCode);
            if (app == NULL) {
                app = appointmentFindByFiscalCode(res->evening, fiscalCode);
            }
        }
    }
    return app;
}

/* Aggiunge in coda un elemento Appointment a res, nella fascia oraria concordata */
void addReservation(Reservation *res, Appointment app) {
    if ((*res) != NULL) {
        if (app != NULL) {
            switch (app->slot) {
                case MORNING:
                    (*res)->morning = appointmentAppendToList((*res)->morning, app);
                    break;
                case AFTERNOON:
                    (*res)->afternoon = appointmentAppendToList((*res)->afternoon, app);
                    break;
                case EVENING:
                    (*res)->evening = appointmentAppendToList((*res)->evening, app);
                    break;
                default:
                    break;
            }
        }
    }
}


