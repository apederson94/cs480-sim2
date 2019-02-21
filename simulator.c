#include "dataStructures.h"
#include "timer.h"
#include "scheduler.h"
#include "booleans.h"
#include "strUtils.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int simulate(struct simAction *actionsList, struct configValues *settings, struct logEntry *logList)
{
    int numApps, runningApp, timeSec, timeUsec, totalTime, cycleTime;
    char *type, *line;
    struct PCB *controlBlock;
    struct PCB **pcbList;
    struct simAction *programCounter;
    struct timeval startTime;
    struct timeval runtime;
    bool programsToRun = TRUE;
    bool logToMon = FALSE;
    bool logToFile = FALSE;

    line = calloc(100, sizeof(char));

    if (strCmp(settings->logTo, "Both"))
    {
        logToMon = TRUE;
        logToFile = TRUE;
    }
    else if (strCmp(settings->logTo, "Monitor"))
    {
        logToMon = TRUE;
    }
    else if (strCmp(settings->logTo, "File"))
    {
        logToFile = TRUE;
    }

    logIt("===============\nSimulator Start\n===============\n\n", logList, logToMon, logToFile);

    gettimeofday(&startTime, NULL);

    sprintf(line, "[%lf] OS: System Start\n", (double) 0);

    logIt(line, logList, logToMon, logToFile);

    sprintf(line, "[%lf] OS: Creating Process Control Blocks\n", tv2double(execTime(startTime)));

    logIt(line, logList, logToMon, logToFile);

    numApps = countApplications(actionsList);

    pcbList = (struct PCB **) calloc(numApps, sizeof(struct PCB));

    createPCBList(pcbList, actionsList, settings);

    sprintf(line, "[%lf] OS: All processes initialized in \"new\" state\n", tv2double(execTime(startTime)));

    logIt(line, logList, logToMon, logToFile);

    setStatesReady(pcbList, numApps);

    sprintf(line, "[%lf] OS: All processes now set to state \"ready\"\n", tv2double(execTime(startTime)));

    logIt(line, logList, logToMon, logToFile);

    runningApp = scheduleNext(pcbList, settings->cpuSched, numApps);

    sprintf(line, "[%lf] Process %d selected with %dms remaining\n\n", tv2double(execTime(startTime)), runningApp, pcbList[runningApp]->timeRemaining);

    logIt(line, logList, logToMon, logToFile);

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

                sprintf(line, "[%lf] Process: %d, %s %s start\n", tv2double(execTime(startTime)), controlBlock->processNum, programCounter->operationString, type);

                logIt(line, logList, logToMon, logToFile);

                runFor(runtime);

                sprintf(line, "[%lf] Process: %d, %s %s end\n", tv2double(execTime(startTime)), controlBlock->processNum, programCounter->operationString, type);

                logIt(line, logList, logToMon, logToFile);

                controlBlock->timeRemaining -= (timeSec*MS_PER_SEC + timeUsec/USEC_PER_MS);
            }

            programCounter = programCounter->next;
            
        }

        if (controlBlock->timeRemaining == 0)
        {
            controlBlock->state="exit";

            sprintf(line, "\n[%lf] Process: %d ended and set in \"exit\" state\n", tv2double(execTime(startTime)), controlBlock->processNum);
            
            logIt(line, logList, logToMon, logToFile);
        }        

        runningApp = scheduleNext(pcbList, settings->cpuSched, numApps);

        if (runningApp == ALL_PROGRAMS_DONE)
        {
            programsToRun = FALSE;
        }

    }

    sprintf(line, "[%lf] OS: System stop\n\n", tv2double(execTime(startTime)));

    logIt(line, logList, logToMon, logToFile);

    sprintf(line, "=========================\nEnd Simulation - Complete\n=========================\n");

    logIt(line, logList, logToMon, logToFile);

    freePCBs(pcbList, numApps);
    free(line);

    return 0;
}