#include <stdlib.h>
#include <stdio.h>
#include "strUtils.h"
#include "booleans.h"
#include "dataStructures.h"
#include "errors.h"

//ALL FILE-RELATED UTILITIES

//READS AND STORES ALL VALUES FROM CONFIG FILE
int readConfigFile(char *fileName, struct configValues *settings) 
{
    char line[100];
    int pos, len, valIter, num;
    float ver;
    FILE *config = fopen(fileName, "r");

    //OPEN CONFIG FILE AND ENSURE IT DOES NOT FAIL
    if (!config) {
        return CONFIG_FILE_OPEN_ERROR;
    }

    valIter = 0;

    while (fgets(line, 100, config)) 
    {
        char *value = (char*) malloc(sizeof(char) * 100);
        pos = substrPos(line, ":"); 
        if (pos >= 0)
        { 
            len = strLen(line);
            substr(line, pos, len, value);

            removeNonSymbols(value);

            if (strContains(line, "Version/Phase")) 
            {
                
                ver = s2f(value);

                if (ver > 10.0f || ver < 0.0f) {
                    return VERSION_PHASE_VALUE_ERROR;
                }

                settings->ver = ver;

            } 
            else if (strContains(line, "Log File")) 
            {
                

                if (strCmp(settings->logTo, "File") 
                || strCmp(settings->logTo, "Both")) 
                {
                    settings->logPath = value;

                } else 
                {
                    settings->logPath = "N/A";
                }

            } 
            else if (strContains(line, "CPU Scheduling Code")) 
            {
                

                if (strCmp(value, "NONE")) 
                {
                    settings->cpuSched = "FCFS-N";

                } 
                else if (!checkCpuSched(value)) 
                {
                    return UNSUPPORTED_CPU_SCHED_ERROR;

                 } 
                 else 
                 {
                    settings->cpuSched = value;
                 }

            } 
            else if (strContains(line, "Quantum Time")) 
            {
                
                num = s2i(value);

                if (num > 100 || num < 0) {
                    return QUANTUM_TIME_VALUE_ERROR;
                }

                settings->quantumTime = num;

            } 
            else if (strContains(line, "Memory Available")) 
            {
                if (!strContains(line, "(KB)")) {
                    return MDF_MEMORY_UNIT_ERROR;
                }

                
                num = s2i(value);

                if (num < 0 || num > 102400) 
                {
                    return MEMORY_AVAIL_VALUE_ERROR;
                }

                settings->memoryAvailable = s2i(value);

            } 
            else if (strContains(line, "Processor Cycle Time")) 
            {
                
                num = s2i(value);

                if (num < 1 || num > 1000) 
                {
                    return CPU_CYCLE_TIME_VALUE_ERROR;
                }

                settings->cpuCycleTime = num;

            } 
            else if (strContains(line, "I/O Cycle Time")) 
            {
                
                num = s2i(value);

                if (num < 1 || num > 10000) 
                {
                    return IO_CYCLE_TIME_VALUE_ERROR;
                }

                settings->ioCycleTime = num;

            } 
            else if (strContains(line, "Log To")) 
            {
                

                if (!checkLogTo(value)) 
                {
                    return INVALID_LOG_TO_ERROR;
                }
                settings->logTo = value;

            } 
            else if (strContains(line, "File Path:")) 
            {
                

                if (strContains(value, "/")) {
                    return MDF_LOCATION_ERROR;
                }

                settings->mdfPath = value;
            }
        }

        valIter++;
    }

    fclose(config);

    return 0;
}

//READS ALL INFORMATION FROM META-DATA FILE
int readMetaDataFile(char *fileName, struct simAction *firstAction) 
{
    char line[100];
    int lineLength, pos, cmdIter, setDataResult;
    cmdIter = 0;
    char command[40];
    struct simAction *current = (struct simAction*) malloc(sizeof(struct simAction));
    
    //OPENS METADATA FILE AND ENSURE IT DID NOT FAIL
    FILE *mdf = fopen(fileName, "r");

    if (!mdf) {
        return MDF_OPEN_ERROR;
    }

    current = firstAction;

    while (fgets(line, 100, mdf)) 
    {
        if (!strContains(line, "Start Program Meta-Data") && !strContains(line, "End Program Meta-Data")) 
        {
            lineLength = strLen(line);

            for (pos = 0; pos < lineLength; pos++) 
            {
                command[cmdIter] = line[pos];
                cmdIter++;

                if (line[pos] == ';') 
                {
                    struct simAction *next = (struct simAction*) malloc(sizeof(struct simAction));
                    command[cmdIter] = '\0';
                    cmdIter = 0;

                    setDataResult = setActionData(command, current);

                    if (setDataResult > 0) 
                    {
                        return setDataResult;
                    }

                    current->next = next;
                    current = next;
                }
            }
        }
    }

    fclose(mdf);

    return 0;
}