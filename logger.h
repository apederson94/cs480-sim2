#include <time.h>
#include "dataStructures.h"

#ifndef LOGGER
#define LOGGER

void logOp(struct loggedOperation *logList, clock_t execAt, int pNum, char *description);

int createLogFile(char *fileName, struct loggedOperation *head);

void freeLoggedOps(struct loggedOperation *head);

double execTime(clock_t start);

#endif