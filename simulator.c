#include "dataStructures.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int simulate(struct simAction *actionsList, struct configValues *settings)
{
    int numApps;
    int pos;
    struct PCB *controlBlock;
    struct PCB **pcbList;
    struct timeval startTime;

    gettimeofday(&startTime, NULL);

    printf("[%lf], OS: System Start\n", (double) (startTime.tv_sec + (startTime.tv_usec/USEC_PER_SEC)));

    printf("[%lf], OS: Creating Process Control Blocks\n", execTime(startTime));

    numApps = countApplications(actionsList);

    pcbList = (struct PCB **) malloc(sizeof(struct PCB) * numApps);

    createPCBList(pcbList, actionsList);

    printf("[%lf], OS: All processes initialized in \"new\" state\n", execTime(startTime));

    //TODO: GO THROUGH PROCESSES AND SET ALL STATES TO "READY"

    printf("[%lf], OS: All processes now set to state \"ready\"\n", execTime(startTime));

    for (pos = 0; pos < numApps; pos++)
    {
        controlBlock = pcbList[pos];

        printf("%c\n", controlBlock->pc->commandLetter);
    }

    return 0;
}