#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "ui.h"
#include "database.h"
#include "patient.h"
#include "lab.h"
#include "helper.h"
#include "structure.h"

#include <unistd.h>

int main() {

    //run();
    Appointment appList = loadAppointmentList();
    TestReservation test = newTestReservation();

    printf("Before taking the appointments:\n\n");
    printTestReservation(test);
    printAppointmentList(appList);

    labPopulateReservations(&test, appList);

    printf("After taking the appointments:\n");
    printTestReservation(test);

    deleteAppointmentList(appList);
    deleteTestReservation(test);
    return 0;
}
