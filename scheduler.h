#include "dataStructures.h"

#ifndef SCHEDULER
#define SCHEDULER

#define ALL_PROGRAMS_DONE -1

int scheduleNext(struct PCB **pcbList, char *scheduler, int numProcesses);

#endif