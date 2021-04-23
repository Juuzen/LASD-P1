#ifndef s_quarantine_h
#define s_quarantine_h
#include "const.h"


/* DEFINIZIONE STRUTTURA DATI */
struct quarantine {
    char fiscalCode[FISCALCODE_SIZE];
    struct quarantine *next;
};
typedef struct quarantine* Quarantine;


/* FUNZIONI SULLA STRUTTURA DATI */
Quarantine quarantineNewList ();
Quarantine quarantineNewNode (char fiscalCode[]);
void quarantineDeleteNode (Quarantine node);
void quarantineDeleteList (Quarantine qtList);
Quarantine quarantineTailInsert (Quarantine qtList, char fiscalCode[]);
void quarantinePrintNode (Quarantine node);
void quarantinePrintList (Quarantine qtList);


#endif // s_quarantine_h
