#ifndef lab_h
#define lab_h
#include "s_employee.h"
#include "structure.h"

/* FUNZIONI AUSILIARIE */
bool labLoginCheck(Employee emList, int id, char password[]);
void labConfirmAppointments(TestReservation *reservation, Appointment *appList);
void labPopulateReservations(TestReservation *reservation, Appointment appList);


/* FUNZIONI UI */
void labShowTestHistoryUi(int currentDay);
void labManageAppointmentRequestsUi(TestReservation *test);
void labShowReservationUi(TestReservation test);
void labAddReservationUi(TestReservation *test);
void labRemoveReservationUi(TestReservation *test);
void labHandleReservationUi(TestReservation *test);
void labLoginUi(TestReservation *test);
void labMainMenuUi(TestReservation *test);

#endif // lab_h
