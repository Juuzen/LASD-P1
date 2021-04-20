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

#define FILENAMESIZE 21
#define FISCALCODE_SIZE 17
#define PASSWORD_SIZE 20
#define SYMPTOMS_SIZE 400

#define PATIENT_DB "patient_db.txt"
#define LABWORKER_DB "labworker_db.txt"

#define APPOINTMENT_DB "appointment_db.txt"

#endif // const_h
