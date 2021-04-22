#ifndef lab_h
#include "structure.h"

bool labLoginCheck(LabWorker wkList, int workerId, char password[]);

Appointment labPopulateReservations(TestReservation *reservation, Appointment appList);

#endif // lab_h
