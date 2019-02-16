#include "mathUtils.h"

//ALL MATH UTILS

//RAISES AN INT BASE NUMBER TO A POWER AND RETURNS RESULT
float raiseToPower(int base, int power)
{
    float value = 1;

    if (power > 0) 
    {
        for (int currentPower = 0; currentPower < power; currentPower++) 
        {
            value *= base;
        }
    }
    else
    {
        for (int currentPower = 0; currentPower > power; currentPower--) 
        {
            value /= base;
        }
    }
    

    return value;
}