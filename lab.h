#ifndef lab_h
#define lab_h
#include "s_employee.h"
#include "s_appointment.h"
#include "s_reservation.h"
#include "structure.h"

/* FUNZIONI AUSILIARIE */
bool labLoginCheck(Employee emList, int id, char password[]);
void labConfirmAppointments(Reservation *res, Appointment *apList);
void labPopulateReservations(Reservation *res, Appointment apList);


/* FUNZIONI UI */
void labShowTestHistoryUi(int currentDay);
void labManageAppointmentRequestsUi(Reservation *res);
void labShowReservationUi(Reservation res);
void labAddReservationUi(Reservation *res);
void labRemoveReservationUi(Reservation *res);
void labHandleReservationUi(Reservation *res);
void labLoginUi(Reservation *res);
void labMainMenuUi(Reservation *res);

#endif // lab_h
