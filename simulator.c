#include "dataStructures.h"
#include "timer.h"
#include "scheduler.h"
#include "booleans.h"
#include "strUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int simulate(struct simAction *actionsList, struct configValues *settings)
{
    int numApps, runningApp, timeSec, timeUsec, totalTime, cycleTime;
    char *type;
    struct PCB *controlBlock;
    struct PCB **pcbList;
    struct simAction *programCounter;
    struct timeval startTime;
    struct timeval runtime;
    bool programsToRun = TRUE;

    gettimeofday(&startTime, NULL);

    printf("[%lf] OS: System Start\n", (double) 0);

    printf("[%lf] OS: Creating Process Control Blocks\n", tv2double(execTime(startTime)));

    numApps = countApplications(actionsList);

    pcbList = (struct PCB **) calloc(numApps, sizeof(struct PCB));

    createPCBList(pcbList, actionsList, settings);

    printf("[%lf] OS: All processes initialized in \"new\" state\n", tv2double(execTime(startTime)));

    setStatesReady(pcbList, numApps);

    printf("[%lf] OS: All processes now set to state \"ready\"\n", tv2double(execTime(startTime)));

    runningApp = scheduleNext(pcbList, settings->cpuSched, numApps);

    printf("[%lf], Process %d selected with %dms remaining\n\n", tv2double(execTime(startTime)), runningApp, pcbList[runningApp]->timeRemaining);

    while (programsToRun)
    {

        controlBlock = pcbList[runningApp];
        programCounter = pcbList[runningApp]->pc;

        while (!strCmp(programCounter->operationString, "end"))
        {
            if (programCounter->commandLetter != 'A')
            {
                if (programCounter->commandLetter == 'P')
                {
                    cycleTime = settings->cpuCycleTime;
                    type = "operation";
                }
                else if (programCounter->commandLetter == 'I'
                || programCounter->commandLetter == 'O')
                {
                    cycleTime = settings->ioCycleTime;

                    if (programCounter->commandLetter == 'I')
                    {
                        type = "input";
                    }
                    else
                    {
                        type = "output";
                    }
                }

                totalTime = programCounter->assocVal * cycleTime;
                timeSec =  totalTime / MS_PER_SEC;
                timeUsec = (totalTime % MS_PER_SEC) * USEC_PER_MS;
                runtime.tv_sec = timeSec;
                runtime.tv_usec = timeUsec;

                printf("[%lf] Process: %d, %s %s start\n", tv2double(execTime(startTime)), controlBlock->processNum, programCounter->operationString, type);

                runFor(runtime);

                printf("[%lf] Process: %d, %s %s end\n", tv2double(execTime(startTime)), controlBlock->processNum, programCounter->operationString, type);

                controlBlock->timeRemaining -= (timeSec*MS_PER_SEC + timeUsec/USEC_PER_MS);
            }

            programCounter = programCounter->next;
            
        }

        if (controlBlock->timeRemaining == 0)
        {
            controlBlock->state="exit";
            printf("\n[%lf] Process: %d ended and set in \"exit\" state\n", tv2double(execTime(startTime)), controlBlock->processNum);
        }        

        runningApp = scheduleNext(pcbList, settings->cpuSched, numApps);

        if (runningApp == ALL_PROGRAMS_DONE)
        {
            programsToRun = FALSE;
        }

    }

    printf("[%lf] OS: System stop\n\n");

    return 0;
}