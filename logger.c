#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "errors.h"
#include "strUtils.h"
#include "dataStructures.h"

void appendToLog(struct logEntry *logList, char *entry)
{
    struct logEntry *listIter;
    listIter = logList;

    listIter->next = (struct logEntry *) malloc(sizeof(struct logEntry));
    listIter->next->entry = entry;

    free(listIter);
}

void appendSettingsToLog(struct logEntry *logList, struct configValues *settings)
{
    char *version, *program, *cpuSched, *quantum, *memAvail, *cpuCycle, *ioCycle, *logTo, *logPath;
    version = malloc(sizeof(settings->ver) * sizeof(char));
    program = malloc(strLen(settings->mdfPath) * sizeof(char));
    cpuSched = malloc(strLen(settings->cpuSched) * sizeof(char));
    quantum = malloc(sizeof(settings->quantumTime) * sizeof(char));
    memAvail = malloc(sizeof(settings->memoryAvailable) * sizeof(char));
    cpuCycle = malloc(sizeof(settings->cpuCycleTime) * sizeof(char));
    ioCycle = malloc(sizeof(settings->ioCycleTime) * sizeof(char));
    logTo = malloc(strLen(settings->logTo) * sizeof(char));
    logPath = malloc(strLen(settings->logPath) * sizeof(char));
    char *all[9] = {version, program, cpuSched, quantum, memAvail, cpuCycle, ioCycle, logTo, logPath};
    int pos;

    sprintf(version, "Version                : %f\n", settings->ver);
    sprintf(program, "Program file name      : %s\n", settings->mdfPath);
    sprintf(cpuSched, "CPU schedule selection : %s\n", settings->cpuSched);
    sprintf(quantum, "Quantum time           : %d\n", settings->quantumTime);
    sprintf(memAvail, "Memory Available       : %d\n", settings->memoryAvailable);
    sprintf(cpuCycle, "Process cycle rate     : %d\n", settings->cpuCycleTime);
    sprintf(ioCycle, "I/O cycle rate         : %d\n", settings->ioCycleTime);
    sprintf(logTo, "Log to selection       : %s\n", settings->logTo);
    sprintf(logPath, "Log file name          : %s\n\n", settings->logPath);

    for (pos = 0; pos < 9; pos++)
    {
        appendToLog(logList, all[pos]);
        free(all[pos]);
    }
}

void appendSimActionsToLog(struct logEntry *logList, struct simAction *head)
{
    struct simAction *actionsIter = head;
    char *cmd, *opString, *assocVal;
    cmd = malloc(sizeof(char));
    
    while (actionsIter->next)
    {
        opString = malloc(sizeof(char) * strLen(actionsIter->operationString));
        assocVal = malloc(sizeof(char) * sizeof(actionsIter->assocVal));
        sprintf(cmd, "Op code letter: %c\n", actionsIter->commandLetter);
        sprintf(opString, "Op code name  : %s\n", actionsIter->operationString);
        sprintf(assocVal, "Op code value : %d\n\n", actionsIter->assocVal);
        actionsIter = actionsIter->next;
        appendToLog(logList, cmd);
        appendToLog(logList, opString);
        appendToLog(logList, assocVal);
        free(opString);
        free(assocVal);
    }

    free(cmd);
    free(actionsIter);
}

int createLogFile(char *fileName, struct logEntry *head)
{
    struct logEntry *listIter = (struct logEntry *) malloc(sizeof(struct logEntry));
    FILE *logFile = fopen(fileName, "w");

    if (!logFile)
    {
        return LOG_OPEN_ERROR;
    }

    listIter = head;

    while (listIter->next)
    {
        fputs(listIter->entry, logFile);
    }

    free(listIter);

    return 0;
}

void freeLoggedOps(struct logEntry *head)
{
    struct logEntry *entry = head;

    while (entry->next)
    {
        entry = head;
        head = head->next;
        free(entry);
    }
}