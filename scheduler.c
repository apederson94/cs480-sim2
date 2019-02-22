#include "dataStructures.h"
#include "strUtils.h"
#include "errors.h"
#include <stdio.h>

/*
    basic scheduler that supports:
        * FCFS-N
*/
int scheduleNext(struct PCB **pcbList, char *scheduler, int numProcesses)
{
    int pcbIter;

    //logic for dealing with "FCFS-N" scheduling type
    if (strCmp(scheduler, "FCFS-N"))
    {

        //iterates PCB array
        for (pcbIter = 0; pcbIter < numProcesses; pcbIter++)
        {

            //if time remaining isn't 0, select the first process in the array
            if (pcbList[pcbIter]->timeRemaining != 0)
            {
                return pcbList[pcbIter]->processNum;
            }
        }
    
        //if no program has time remaining, return ALL_PROGRAMS_DONE code
        return ALL_PROGRAMS_DONE;
        
    }

    return SCHEDULER_NOT_AVAILABLE;
}