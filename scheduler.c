#include "dataStructures.h"
#include "strUtils.h"
#include "errors.h"
#include "scheduler.h"
#include <stdio.h>

int scheduleNext(struct PCB **pcbList, char *scheduler, int numProcesses)
{
    int pcbIter;
    if (strCmp(scheduler, "FCFS-N"))
    {
        for (pcbIter = 0; pcbIter < numProcesses; pcbIter++)
        {
            if (pcbList[pcbIter]->timeRemaining != 0)
            {
                return pcbList[pcbIter]->processNum;
            }
        }
    
        return ALL_PROGRAMS_DONE;
        
    }

    return 0;
}