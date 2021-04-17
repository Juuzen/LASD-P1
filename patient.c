#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "database.h"
#include "patient.h"
#include "structure.h"



bool loginCheck(Patient ptList, char fiscalCode[], char password[]) {
    if (ptList == NULL) {
        return false;
    }
    else {
        if (strcmp(ptList->fiscalCode, fiscalCode) == 0) {
            if (strcmp(ptList->password, password) == 0) {
                return true;
            }
            else {
                return false;
            }
        }
        else return loginCheck(ptList->next, fiscalCode, password);
    }
}

bool isPatientRegistered(Patient ptList, char fiscalCode[]) {
    if (ptList == NULL) {
        return false;
    }

    else {
        return ((strcmp(ptList->fiscalCode, fiscalCode)) == 0) ? true : isPatientRegistered(ptList->next, fiscalCode);
    }
}

// TIP: Nella funzione chiamante usa &ptList
bool patientRegister(Patient* ptList, char fiscalCode[], char password[]) {
    bool response = false;
    if (!isPatientRegistered(*ptList, fiscalCode)) {
        *ptList = patientInsert(*ptList, fiscalCode, password);
        savePatient(fiscalCode, password);
        response = true;
    }

    return response;
}



