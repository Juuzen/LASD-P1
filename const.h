#ifndef const_h
#define const_h

#ifdef _WIN32
#define OS_WINDOWS
#endif

#ifdef __linux__
#define OS_LINUX
#endif

#ifdef __APPLE__
#define OS_APPLE
#endif

#define FISCALCODE_SIZE 17
#define PASSWORD_SIZE 21
#define SYMPTOMS_SIZE 400
#define RESPONSE_SIZE 9
#define SLOT_CAPABILITY 2

#define PATIENT_DB "patient_db.txt"
#define EMPLOYEE_DB "employee_db.txt"
#define APPOINTMENT_DB "appointment_db.txt"
#define TESTRESULT_DB "testresult_db.txt"
#define QUARANTINE_DB "quarantine_db.txt"

#define ERR_READING "Error in reading from file."
#define ERR_WRITING "Error in writing into the file."
#define ERR_FILEACCESS "The database does not exist or could not be opened."

#define PAUSE_DEFAULT "Press ENTER key to go back..."

typedef enum timeSlot { MORNING, AFTERNOON, EVENING } timeSlot;

#endif // const_h
