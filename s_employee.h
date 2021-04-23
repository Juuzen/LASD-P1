#ifndef s_employee_h
#define s_employee_h
#include "const.h"


/* DEFINIZIONE STRUTTURA DATI */
struct employee {
    int id;
    char password[PASSWORD_SIZE];
    struct employee* next;
};
typedef struct employee* Employee;


/* FUNZIONI SULLA STRUTTURA DATI */
Employee employeeNewList();
Employee employeeNewNode(int id, char password[]);
void employeeFreeNode(Employee emNode);
void employeeFreeList(Employee emList);
Employee employeeTailInsert(Employee emList, int id, char password[]);
void employeePrintNode(Employee emNode);
void employeePrintList(Employee emList);


#endif // s_employee_h
