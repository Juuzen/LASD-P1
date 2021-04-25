#ifndef s_result_h
#define s_result_h

#include <stdbool.h>
#include "const.h"


/* DEFINIZIONE STRUTTURA DATI */
struct testResult {
    char fiscalCode[FISCALCODE_SIZE];
    char response[RESPONSE_SIZE];
    int day;

    struct testResult* next;
};
typedef struct testResult* TestResult;


/* FUNZIONI SULLA STRUTTURA DATI */
TestResult testResultNewList();
TestResult testResultNewNode(char fiscalCode[], char response[], int day);
void testResultFreeNode(TestResult rsNode);
void testResultFreeList(TestResult rsList);
TestResult testResultTailInsert (TestResult rsList, char fiscalCode[], char response[], int day);
void testResultPrintNode(TestResult rsNode);
void testResultPrintList(TestResult rsList, bool chronoOrder);
TestResult testResultFilterByDay (TestResult rsList, int day);
TestResult testResultFilterByFiscalCode (TestResult rsList, char fiscalCode[]);

#endif // s_result_h
