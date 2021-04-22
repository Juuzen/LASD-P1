#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab.h"
#include "structure.h"
#include "database.h"

bool labLoginCheck(LabWorker wkList, int workerId, char password[]) {
    if (wkList == NULL) return false;
    else {
        if ((wkList->id == workerId) && (strcmp(wkList->password, password) == 0)) return true;
        else return labLoginCheck(wkList->next, workerId, password);
    }
}



void labConfirmAppointments(TestReservation *reservation, Appointment *appList) {
    labPopulateReservations(reservation, (*appList));
    deleteAppointmentList((*appList));
    (*appList) = NULL;

    dropAppointmentDB();
}

void labPopulateReservations(TestReservation *reservation, Appointment appList) {
    if ((*reservation) == NULL) {
        (*reservation) = newTestReservation();
    }

    if (appList != NULL) {
        if (!isTimeSlotFull((*reservation), appList->slot)) {
            addReservation(reservation, cloneAppointment(appList));
        }

        labPopulateReservations(reservation, appList->next);
    }
}
