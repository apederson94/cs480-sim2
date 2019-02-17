#include <time.h>
#include <stdio.h>
#include "logger.h"
#include "errors.h"

void appendToLog(struct loggedOperation *logList, char *description)
{
    struct loggedOperation *tmp = (struct loggedOperation *) malloc(sizeof(struct loggedOperation));
    struct loggedOperation *listIter = (struct loggedOperation *) malloc(sizeof(struct loggedOperation));

    tmp->description = description;

    listIter = logList;

    while (listIter->next)
    {
        listIter = listIter->next;
    }

    listIter->next = tmp;

    free(tmp);
    free(listIter);
}

void appendSettingsToLog(struct loggedOperation *logList, struct configValues *settings)
{
    char *version, *program, *cpuSched, *quantum, *memAvail, *cpuCycle, *ioCycle, *logTo, *logPath;
    char *all[9];
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

void appendSimActionsToLog(struct loggedOperation *logList, struct simAction *head)
{
    struct simAction *tmp = head;
    char *cmd, *opString, *assocVal;
    
    while (tmp->next)
    {
        sprintf(cmd, "Op code letter: %c\n", tmp->commandLetter);
        sprintf(opString, "Op code name  : %s\n", tmp->operationString);
        sprintf(assocVal, "Op code value : %d\n\n", tmp->assocVal);
        tmp = tmp->next;
        appendToLog(logList, cmd);
        appendToLog(logList, opString);
        appendToLog(logList, assocVal);
    }

    free(tmp);
}

int createLogFile(char *fileName, struct loggedOperation *head)
{
    struct loggedOperation *listIter = (struct loggedOperation *) malloc(sizeof(struct loggedOperation));
    FILE *logFile = fopen(fileName, 'w');

    if (!logFile)
    {
        return LOG_OPEN_ERROR;
    }

    listIter = head;

    while (listIter->next)
    {
        fprintf(logFile, listIter->description);
    }

    free(listIter);

    return 0;
}

void freeLoggedOps(struct loggedOperation *head)
{
    struct loggedOperation *tmp;

    while (head->next)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

double execTime(clock_t start)
{
    return clock() - start;
}