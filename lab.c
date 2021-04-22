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

void addReservation(TestReservation *reservation, Appointment app) {
    if ((*reservation) != NULL) {
        if (app != NULL) {
            switch (app->slot) {
                case MORNING:
                    (*reservation)->morning = appointmentAppend((*reservation)->morning, app);
                    break;
                case AFTERNOON:
                    (*reservation)->afternoon = appointmentAppend((*reservation)->afternoon, app);
                    break;
                case EVENING:
                    (*reservation)->evening = appointmentAppend((*reservation)->evening, app);
                    break;
                default:
                    break;
            }
        }
    }
}

Appointment labPopulateReservations(TestReservation *reservation, Appointment appList) {
    if ((*reservation) == NULL) {
        (*reservation) = newTestReservation();
    }

    if (appList != NULL) {

        if (!isTimeSlotFull((*reservation), appList->slot)) {
                /*
            switch (appList->slot) {
                case MORNING:
                    (*reservation)->morning = appointmentAppend((*reservation)->morning, appList);
                    break;
                case AFTERNOON:
                    (*reservation)->afternoon = appointmentAppend((*reservation)->afternoon, appList);
                    break;
                case EVENING:
                    (*reservation)->evening = appointmentAppend((*reservation)->evening, appList);
                    break;
                default:
                    break;
            }
            */
            addReservation(reservation, appList);
            appList->next = labPopulateReservations(reservation, appList->next);
            return appList->next;
        }

        else {
            appList->next = labPopulateReservations(reservation, appList->next);
            return appList;
        }

        labPopulateReservations(reservation, appList->next);
    }

    else return NULL;
}
