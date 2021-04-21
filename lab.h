#ifndef lab_h
#include "structure.h"

bool labLoginCheck(LabWorker wkList, int workerId, char password[]);

TestReservation acceptAppointment(TestReservation reservation, Appointment app);

#endif // lab_h
