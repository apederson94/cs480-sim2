#include "timer.h"
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

struct timeval execTime(struct timeval start)
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

    end.tv_sec = secDiff;
    end.tv_usec = usecDiff;

    return end;
}

void runFor(struct timeval time)
{
    pthread_t threadId;

    //pass in time to run for, and clock start time for program
    pthread_create(&threadId, NULL, threadTimer, &time);

    pthread_join(threadId, NULL);
}

void * threadTimer(void *args)
{
    struct timeval *runtime = (struct timeval*) args;
    struct timeval time;
    struct timeval start;
    int secDiff, usecDiff;

    gettimeofday(&start, NULL);
    gettimeofday(&time, NULL);

    secDiff = time.tv_sec - start.tv_sec;
    usecDiff = time.tv_sec - start.tv_sec;

    while(secDiff < runtime->tv_sec || usecDiff < runtime->tv_usec)
    {
        gettimeofday(&time, NULL);
        secDiff = time.tv_sec - start.tv_sec;
        usecDiff = time.tv_usec - start.tv_usec;

        if (usecDiff < 0)
        {
            usecDiff += USEC_PER_SEC;
            secDiff--;
        }
    }

    return 0;
}

double tv2double(struct timeval tv)
{
    return ((double)tv.tv_sec) + (((double)tv.tv_usec)/USEC_PER_SEC);
}