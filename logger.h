#include <time.h>
#include "dataStructures.h"

#ifndef LOGGER
#define LOGGER

//stores all operations in a linked list
struct loggedOperation
{
    char *description;
    struct loggedOperation *next;
};

void appendToLog(struct loggedOperation *logList, char *description);

void appendSettingsToLog(struct loggedOperation *logList, struct configValues *settings);

void appendSimActionsToLog(struct loggedOperation *logList, struct simAction *head);

int createLogFile(char *fileName, struct loggedOperation *head);

void freeLoggedOps(struct loggedOperation *head);

double execTime(struct timespec start);

#endif