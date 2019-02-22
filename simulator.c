#include "dataStructures.h"
#include "timer.h"
#include "scheduler.h"
#include "booleans.h"
#include "strUtils.h"
#include "logger.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/*
    simulates an operating system and the actions that run on it:
        * inputs are a simAction linked list, configValues struct, and a logEvent linked list
        * starts OS
        * builds array of PCBs
        * schedules next application to be run
        * runs applications
        * returns 0 when finished running
*/
int simulate(struct simAction *actionsList, struct configValues *settings, struct logEvent *logList)
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

    //allocating memory for line
    line = calloc(100, sizeof(char));

    //settings flags for logging
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

    //getting start time of OS
    gettimeofday(&startTime, NULL);

    sprintf(line, "[%lf] OS: System Start\n", (double) 0);
    logIt(line, logList, logToMon, logToFile);

    sprintf(line, "[%lf] OS: Creating Process Control Blocks\n", tv2double(execTime(startTime)));
    logIt(line, logList, logToMon, logToFile);

    //counting number of applications in actions list
    numApps = countApplications(actionsList);

    //allocating memory for PCB array
    pcbList = (struct PCB **) calloc(numApps, sizeof(struct PCB));

    //creates PCB array
    createPCBList(pcbList, actionsList, settings);

    sprintf(line, "[%lf] OS: All processes initialized in \"new\" state\n", tv2double(execTime(startTime)));
    logIt(line, logList, logToMon, logToFile);

    //setting state to ready for all PCBs in PCB array
    setStatesReady(pcbList, numApps);

    sprintf(line, "[%lf] OS: All processes now set to state \"ready\"\n", tv2double(execTime(startTime)));
    logIt(line, logList, logToMon, logToFile);

    //schedules the next app to be run
    runningApp = scheduleNext(pcbList, settings->cpuSched, numApps);

    //error checking
    if (runningApp < 0)
    {
        return runningApp;
    }

    sprintf(line, "[%lf] Process %d selected with %dms remaining\n\n", tv2double(execTime(startTime)), runningApp, pcbList[runningApp]->timeRemaining);
    logIt(line, logList, logToMon, logToFile);

    while (programsToRun)
    {

        //sets current PCB and current program counter
        controlBlock = pcbList[runningApp];
        programCounter = controlBlock->pc;

        //iterates until A(end) occurs
        while (!strCmp(programCounter->operationString, "end"))
        {

            //if program counter's command letter is not 'A'
            if (programCounter->commandLetter != 'A')
            {

                //if program counter's command letter is 'P
                if (programCounter->commandLetter == 'P')
                {
                    cycleTime = settings->cpuCycleTime;
                    type = "operation";
                }

                //if program counter's command letter is 'I' or 'O'
                else if (programCounter->commandLetter == 'I'
                || programCounter->commandLetter == 'O')
                {
                    cycleTime = settings->ioCycleTime;

                    //setting the correct type for each command letter
                    if (programCounter->commandLetter == 'I')
                    {
                        type = "input";
                    }
                    else
                    {
                        type = "output";
                    }
                }

                //creating time time values and setting timeval data
                totalTime = programCounter->assocVal * cycleTime;
                timeSec =  totalTime / MS_PER_SEC;
                timeUsec = (totalTime % MS_PER_SEC) * USEC_PER_MS;
                runtime.tv_sec = timeSec;
                runtime.tv_usec = timeUsec;

                sprintf(line, "[%lf] Process: %d, %s %s start\n", tv2double(execTime(startTime)), controlBlock->processNum, programCounter->operationString, type);
                logIt(line, logList, logToMon, logToFile);

                //runs app for amount of time stored in runtime struct
                runFor(runtime);

                sprintf(line, "[%lf] Process: %d, %s %s end\n", tv2double(execTime(startTime)), controlBlock->processNum, programCounter->operationString, type);
                logIt(line, logList, logToMon, logToFile);

                //subtracts from timeRemaining how much time the app was run for
                controlBlock->timeRemaining -= (timeSec*MS_PER_SEC + timeUsec/USEC_PER_MS);
            }

            //iterates program counter
            programCounter = programCounter->next;
            
        }

        //if timeRemaining is zero logic
        if (controlBlock->timeRemaining == 0)
        {

            //sets state to exit for control block
            controlBlock->state="exit";

            sprintf(line, "\n[%lf] Process: %d ended and set in \"exit\" state\n", tv2double(execTime(startTime)), controlBlock->processNum);
            logIt(line, logList, logToMon, logToFile);
        }        

        //schedules the next app to be run
        runningApp = scheduleNext(pcbList, settings->cpuSched, numApps);

        //if ALL_PROGRAMS_DONE received from scheduler, set programsToRun to FALSE
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