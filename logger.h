#include <sys/time.h>
#include "dataStructures.h"

#ifndef LOGGER
#define LOGGER

//stores all operations in a linked list
struct logEntry
{
    char *entry;
    struct logEntry *next;
};

void appendToLog(struct logEntry *logList, char *entry);

void appendSettingsToLog(struct logEntry *logList, struct configValues *settings);

void appendSimActionsToLog(struct logEntry *logList, struct simAction *head);

int createLogFile(char *fileName, struct logEntry *head);

void freeLoggedOps(struct logEntry *head);

void printLog(struct logEntry *logList);

#endif