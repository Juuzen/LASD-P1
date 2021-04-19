#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "ui.h"
#include "database.h"
#include "patient.h"

int main() {

    //run();

    /*
    Appointment app = newAppointmentNode("100", MORNING, NULL);
    Appointment app2 = newAppointmentNode("200", AFTERNOON, "Tosse, catarro");
    printAppointmentNode(app);
    printAppointmentNode(app2);
    deleteAppointmentNode(app);
    deleteAppointmentNode(app2);
    */

    Appointment appList = loadAppointmentList();
    printAppointmentList(appList);

    return 0;
}
