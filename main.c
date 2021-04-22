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
    //FIXME: aggiustare i puntatori in TestReservation durante il popolamento
    //run();
    Appointment appList = loadAppointmentList();
    TestReservation test = newTestReservation();

    appList = labPopulateReservations(&test, appList);

    labRemoveReservationUi(&test);

    printTestReservation(test);
    return 0;
}
