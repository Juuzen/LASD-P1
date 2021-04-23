#ifndef s_reservation_h
#define s_reservation_h
#include <stdbool.h>
#include "s_appointment.h"
#include "const.h"


/* DEFINIZIONE STRUTTURA DATI */
struct reservation {
    Appointment morning;
    Appointment afternoon;
    Appointment evening;
    int currentDay;
};
typedef struct reservation* Reservation;


/* FUNZIONI SULLA STRUTTURA DATI */
Reservation newReservation();
void freeReservation(Reservation res);
void printReservation(Reservation res);
Appointment searchReservationByFiscalCode(Reservation res, char fiscalCode[]);
void addReservation(Reservation *res, Appointment app);


/* FUNZIONI AUSILIARIE */
bool isTimeSlotFull(Reservation res, timeSlot slot);

#endif // s_reservation_h
