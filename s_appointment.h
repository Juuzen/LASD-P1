#ifndef s_appointment_h
#define s_appointment_h
#include "const.h"

/* DEFINIZIONE STRUTTURA DATI */


struct appointment {
    char fiscalCode[FISCALCODE_SIZE];
    timeSlot slot;
    char *symptoms;

    struct appointment* next;
};
typedef struct appointment* Appointment;


/* FUNZIONI SULLA STRUTTURA DATI */
Appointment appointmentNewList ();
Appointment appointmentNewNode (char fiscalCode[], timeSlot slot, char symptoms[]);
Appointment appointmentCloneNode (Appointment apNode);
void appointmentFreeNode (Appointment apNode);
void appointmentFreeList (Appointment apList);
Appointment appointmentAppendToList (Appointment apList, Appointment appointment);
Appointment appointmentTailInsert (Appointment apList, char fiscalCode[], timeSlot slot, char symptoms[]);
void appointmentPrintNode (Appointment apNode);
void appointmentPrintList (Appointment apList);
Appointment appointmentFindByFiscalCode (Appointment apList, char fiscalCode[]);
Appointment appointmentDeleteByFiscalCode (Appointment apList, char fiscalCode[]);
int appointmentListCount(Appointment apList);

#endif // s_appointment_h
