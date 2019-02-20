#include "dataStructures.h"
#include "strUtils.h"

int scheduleNext(struct PCB **pcbList, char *schedluer, int numProcesses)
{
    int pcbIter;
    if (strCmp(schedluer, "FCFS-N"))
    {
        for (pcbIter = 0; pcbIter < numProcesses; pcbIter++)
        {
            if (pcbList[pcbIter]->timeRemaining != 0)
            {
                return pcbList[pcbIter]->processNum;
            }
        }
    }

    return 0;
}