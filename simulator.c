#include "dataStructures.h"
#include "timer.h"
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int simulate(struct simAction *actionsList, struct configValues *settings)
{
    int numApps;
    int pos;
    int runningApp;
    struct PCB *controlBlock;
    struct PCB **pcbList;
    struct timeval startTime;

    gettimeofday(&startTime, NULL);

    printf("[%lf], OS: System Start\n", (double) 0);

    printf("[%lf], OS: Creating Process Control Blocks\n", execTime(startTime));

    numApps = countApplications(actionsList);

    pcbList = (struct PCB **) calloc(numApps, sizeof(struct PCB));

    createPCBList(pcbList, actionsList, settings);

    printf("[%lf], OS: All processes initialized in \"new\" state\n", execTime(startTime));

    setStatesReady(pcbList, numApps);

    printf("[%lf], OS: All processes now set to state \"ready\"\n", execTime(startTime));

    runningApp = scheduleNext(pcbList, settings->cpuSched, numApps);

    printf("[%lf], Process %d selected with %dms remaining\n", execTime(startTime), runningApp, pcbList[runningApp]->timeRemaining);

    for (pos = 0; pos < numApps; pos++)
    {
        controlBlock = pcbList[pos];

        printf("%c\n", controlBlock->pc->commandLetter);
        printf("%s\n", controlBlock->state);
    }

    return 0;
}