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

TestReservation acceptAppointment(TestReservation reservation, Appointment app) {
    if (reservation == NULL) {
        reservation = newTestReservation();
    }

    if (app != NULL) {
        if (!isTimeSlotFull(reservation, app->slot)) {
            switch (app->slot) {
                case MORNING:
                    reservation->morning = appointmentAppend(reservation->morning, cloneAppointment(app));
                    break;
                case AFTERNOON:
                    reservation->afternoon = appointmentAppend(reservation->afternoon, cloneAppointment(app));
                    break;
                case EVENING:
                    reservation->evening = appointmentAppend(reservation->evening, cloneAppointment(app));
                    break;
                default:
                    break;
            }
        }
    }
    return reservation;
}
