#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s_employee.h"

/* Restituisce un puntatore Employee = NULL */
Employee employeeNewList() {
    Employee emList = NULL;
    return emList;
}

/* Restituisce, se possibile, un elemento Employee con i valori passati in ingresso */
/* Altrimenti, restituisce NULL */
Employee employeeNewNode(int id, char password[]) {
    Employee emNode = (Employee) calloc(1, sizeof(struct employee));
    if (emNode != NULL) {
        emNode->id = id;
        strcpy(emNode->password, password);
        emNode->next = NULL;
    }

    return emNode;
}

/* Dealloca un elemento Employee, pulendo anche il suo contenuto */
void employeeFreeNode(Employee emNode) {
    if (emNode != NULL) {
        emNode->id = 0;
        memset(emNode->password, '\0', sizeof(emNode->password));
        emNode->next = NULL;
        free(emNode);
    }
}

/* Dealloca una intera lista di elementi Appointment */
void employeeFreeList(Employee emList) {
    if (emList != NULL) {
        employeeFreeList(emList->next);
        employeeFreeNode(emList);
    }
}

/* Alloca un elemento Appointment e lo inserisce in coda ad apList */
Employee employeeTailInsert(Employee emList, int id, char password[]) {
    if (emList == NULL) return employeeNewNode(id, password);
    else {
        emList->next = employeeTailInsert(emList->next, id, password);
        return emList;
    }
}

