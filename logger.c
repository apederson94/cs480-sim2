#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "errors.h"
#include "strUtils.h"
#include "dataStructures.h"

void appendToLog(struct logEntry *logList, char *entry)
{
    struct logEntry *listIter = logList;
    int pos = 0;

    while (listIter->next)
    {
        pos++;
        listIter = listIter->next;
    }

    if (pos > 0)
    {
        //PROGRAM CRASHES HERE "malloc(): memory corruption"
        listIter->next = (struct logEntry *) calloc(1, sizeof(struct logEntry));
        listIter = listIter->next;
        listIter->entry = (char *) calloc(strLen(entry), sizeof(char));
        listIter->entry = entry;
    }

}

void appendSettingsToLog(struct logEntry *logList, struct configValues *settings)
{
    char *version, *program, *cpuSched, *quantum, *memAvail, *cpuCycle, *ioCycle, *logTo, *logPath;
    version = calloc(100, sizeof(char));
    program = calloc(100, sizeof(char));
    cpuSched = calloc(100, sizeof(char));
    quantum = calloc(100, sizeof(char));
    memAvail = calloc(100, sizeof(char));
    cpuCycle = calloc(100, sizeof(char));
    ioCycle = calloc(100, sizeof(char));
    logTo = calloc(100, sizeof(char));
    logPath = calloc(100, sizeof(char));
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
    }
}

void appendSimActionsToLog(struct logEntry *logList, struct simAction *head)
{
    struct simAction *actionsIter = head;
    char *cmd, *opString, *assocVal;
    
    while (actionsIter->next)
    {
        cmd = calloc(100, sizeof(char));
        opString = calloc(100, sizeof(char));
        assocVal = calloc(100, sizeof(char));
        sprintf(cmd, "Op code letter: %c\n", actionsIter->commandLetter);
        sprintf(opString, "Op code name  : %s\n", actionsIter->operationString);
        sprintf(assocVal, "Op code value : %d\n\n", actionsIter->assocVal);
        actionsIter = actionsIter->next;
        appendToLog(logList, cmd);
        appendToLog(logList, opString);
        appendToLog(logList, assocVal);
    }

}

int createLogFile(char *fileName, struct logEntry *head)
{
    struct logEntry *listIter = (struct logEntry *) calloc(1, sizeof(struct logEntry));
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
        free(entry->entry);
        free(entry);
    }
}

void printLog(struct logEntry *logList)
{
    struct logEntry *entry = logList;

    while (entry->next)
    {
        printf("%s\n", entry->entry);
    }

    printf("%s\n", entry->entry);

}