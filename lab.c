#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab.h"
#include "structure.h"

bool labLoginCheck(LabWorker wkList, int workerId, char password[]) {
    if (wkList == NULL) return false;
    else {
        if ((wkList->id == workerId) && (strcmp(wkList->password, password) == 0)) return true;
        else return labLoginCheck(wkList->next, workerId, password);
    }
}

void labPopulateReservations(TestReservation *reservation, Appointment appList) {
    //TODO: gestire il caso in cui reservation sia null
    if ((*reservation) == NULL) {
        (*reservation) = newTestReservation();
    }

    if (appList != NULL) {
        if (!isTimeSlotFull((*reservation), appList->slot)) {
            switch (appList->slot) {
                case MORNING:
                    (*reservation)->morning = appointmentAppend((*reservation)->morning, cloneAppointment(appList));
                    break;
                case AFTERNOON:
                    (*reservation)->afternoon = appointmentAppend((*reservation)->afternoon, cloneAppointment(appList));
                    break;
                case EVENING:
                    (*reservation)->evening = appointmentAppend((*reservation)->evening, cloneAppointment(appList));
                    break;
                default:
                    break;
            }
        }

        labPopulateReservations(reservation, appList->next);
    }
}
