#include "timer.h"
#include <stdlib.h>

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