#ifndef s_patient_h
#define s_patient_h
#include "const.h"

/* DEFINIZIONE STRUTTURA DATI */
typedef struct patient {
    char fiscalCode[FISCALCODE_SIZE];
    char password[PASSWORD_SIZE];
    struct patient* next;
} patient;
typedef patient* Patient;


/* FUNZIONI SULLA STRUTTURA DATI */
Patient patientNewList(); //TODO: deprecare
Patient patientNewNode(char fiscalCode[], char password[]);
Patient patientTailInsert(Patient ptList, char fiscalCode[], char password[]);
void patientFreeNode(Patient ptNode);
void patientFreeList(Patient ptList);
void patientPrintNode(Patient ptNode);
void patientPrintList(Patient ptList);


#endif // s_patient_h
