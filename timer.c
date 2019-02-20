#include "timer.h"
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

double execTime(struct timeval start)
{
    struct timeval end;
    double secDiff;
    double usecDiff;

    gettimeofday(&end, NULL);
    
    secDiff = end.tv_sec - start.tv_sec;
    usecDiff = end.tv_usec - start.tv_usec;

    if (usecDiff < 0)
    {
        usecDiff +=  USEC_PER_SEC;
        secDiff--;
    }

    usecDiff /= USEC_PER_SEC;

    return secDiff + usecDiff;
}

void runFor(double time)
{
    pthread_t threadId;

    //pass in time to run for, and clock start time for program
    pthread_create(threadId, NULL, threadTimer, &time);

    pthread_join(threadId, NULL);
}

void threadTimer(void *args)
{
    struct timeval start;
    gettimeofday(&start, NULL);
    return 0;
}