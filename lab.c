#include <stdio.h>
#include <stdlib.h>
#include "lab.h"
#include "structure.h"

TestingDay acceptAppointment(TestingDay test, Appointment app) {
    if (test == NULL) {
        test = newTestingDay();
    }

    if (app != NULL) {
        if (!isTimeSlotFull(test, app->slot)) {
            switch (app->slot) {
                case MORNING:
                    test->morning = appointmentAppend(test->morning, cloneAppointment(app));
                    break;
                case AFTERNOON:
                    test->afternoon = appointmentAppend(test->afternoon, cloneAppointment(app));
                    break;
                case EVENING:
                    test->evening = appointmentAppend(test->evening, cloneAppointment(app));
                    break;
                default:
                    break;
            }
        }
    }
    return test;
}
