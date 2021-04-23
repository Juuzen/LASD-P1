#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s_employee.h"

Employee employeeNewList() {
    Employee emList = NULL;
    return emList;
}

Employee employeeNewNode(int id, char password[]) {
    Employee emNode = (Employee) calloc(1, sizeof(struct employee));
    if (emNode != NULL) {
        emNode->id = id;
        strcpy(emNode->password, password);
        emNode->next = NULL;
    }

    return emNode;
}

void employeeFreeNode(Employee emNode) {
    if (emNode != NULL) {
        emNode->id = 0;
        memset(emNode->password, '\0', sizeof(emNode->password));
        emNode->next = NULL;
        free(emNode);
    }
}

void employeeFreeList(Employee emList) {
    if (emList != NULL) {
        employeeFreeList(emList->next);
        employeeFreeNode(emList);
    }
}

Employee employeeTailInsert(Employee emList, int id, char password[]) {
    if (emList == NULL) return employeeNewNode(id, password);
    else {
        emList->next = employeeTailInsert(emList->next, id, password);
        return emList;
    }
}

void employeePrintNode(Employee emNode) {
    if (emNode != NULL) {
        printf("%d - %s\n", emNode->id, emNode->password);
    }
}

void employeePrintList(Employee emList) {
    if (emList != NULL) {
        employeePrintNode(emList);
        employeePrintList(emList->next);
    }
}

