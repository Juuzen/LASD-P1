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
    printAppointmentList(appList);
    printTestReservation(test);
    pause("Test 1");

    appList = labPopulateReservations(&test, appList);
    printAppointmentList(appList);
    printTestReservation(test);

    return 0;
}
