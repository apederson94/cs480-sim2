#include <time.h>
#include <stdio.h>
#include "dataStructures.h"
#include "errors.h"

void logOperation(struct loggedOperation *logList, clock_t execAt, int pNum, char *description, struct loggedOperation *next)
{
    struct loggedOperation *tmp = (struct loggedOperation *) malloc(sizeof(struct loggedOperation));
    struct loggedOperation *listIter = (struct loggedOperation *) malloc(sizeof(struct loggedOperation));

    tmp->executedAt = execAt;
    tmp->processNum = pNum;
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

int logToFile(char *fileName, struct loggedOperation *head)
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
        fprintf(logFile, listIter->description, listIter->executedAt, listIter->processNum);
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