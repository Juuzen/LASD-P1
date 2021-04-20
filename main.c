#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "ui.h"
#include "database.h"
#include "patient.h"
#include "lab.h"

int main() {

    //run();

    printf("If you have any symptoms, please provide a concise explanation (max %d characters):\n", SYMPTOMS_SIZE);
    char * symptoms = getSymptoms(stdin);

    printf("%s\n", symptoms);
    /*
    Appointment app1 = newAppointmentNode("100", MORNING, NULL);
    Appointment app2 = newAppointmentNode("200", EVENING, NULL);
    Appointment app3 = newAppointmentNode("300", AFTERNOON, "Tosse, catarro");

    TestingDay day = newTestingDay();
    day = acceptAppointment(day, app1);
    day = acceptAppointment(day, app2);
    day = acceptAppointment(day, app3);

    deleteAppointmentNode(app1);
    deleteAppointmentNode(app2);
    deleteAppointmentNode(app3);

    printTestingDay(day);
    deleteTestingDay(day);
    */
    return 0;
}
