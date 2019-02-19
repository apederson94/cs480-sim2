#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "booleans.h"
#include "strUtils.h"
#include "fileUtils.h"
#include "dataStructures.h"
#include "errors.h"
#include "logger.h"
#include "simulator.h"

//TODO: LOG FILE PRINTING BEFORE CLOSING FILE ALL AT ONCE INCLUDING TIMES OF PRINTING.
    //MAYBE USE A LINKED LIST FOR THIS? WOULD BE AN EASY WAY TO STORE DYNAMIC AMOUNT OF INFORMATION

int main(int argc, char const *argv[]) {

    //VARIABLE DECLARATIONS
    char *fileExt;
    char *fileName = (char*) argv[1];
    char *logDescription;
    struct configValues *settings = (struct configValues*) malloc(sizeof(struct configValues));
    struct simAction *actionsHead = (struct simAction*) malloc(sizeof(struct simAction));
    struct loggedOperation *logList = (struct loggedOperation*) malloc(sizeof(struct loggedOperation));
    struct PCB **pcbList;
    int cfgVal, mdfVal, verificationVal, numApps, simVal;
    struct timespec startTime;
    double runTime;
    bool logToFile = FALSE;
    bool logToMon = FALSE;

    logDescription = malloc(sizeof(char) * 100);

    clock_gettime(NULL, &startTime);
    
    //CORRECT NUMBER OF INPUTS CHECK
    if (argc != 2) 
    {
        displayError(NUM_ARGS_ERROR);
        return 1;
    }

    //INPUT FORMAT CHECK
    fileExt = getFileExt(fileName);
    if (!strCmp(fileExt, ".cnf")) 
    {
        displayError(FILE_TYPE_ERROR);
        return 1;
    }

    //STARTING FILE UPLOAD PROCESS
    runTime = execTime(startTime);
    sprintf(logDescription, "[%ld]     Begin %s upload...\n\n", runTime, fileName);
    printf("%s", logDescription);
    appendToLog(logList, logDescription);




	//READ IN CONFIG FILE VALUES
    cfgVal = readConfigFile(fileName, settings);

    //ERROR CHECKING FOR CONFIG FILE READING
    if (cfgVal > 0) 
    {
        if (settings->logTo)
        {
            if (strCmp(settings->logTo, "Both")
            || strCmp(settings->logTo, "File"))
            {
                createLogFile(settings->logPath, logList);
            }
        }
        displayError(cfgVal);
        return 1;
    }

    if (strCmp(settings->logTo, "Both")
    || strCmp(settings->logTo, "File"))
    {
        logToFile = TRUE;
    }
    else if (strCmp(settings->logTo, "Both")
    || strCmp(settings->logTo, "Monitor"))
    {
        logToMon = TRUE;
    }

    //PRINTING SUCCESS MESSAGE
    runTime = execTime(startTime);
    sprintf(logDescription, "[%lf]     %s uploaded successfully!\n\n", runTime, fileName);
    printf("%s", logDescription);
    appendToLog(logList, logDescription);

    //PRINTING CONFIG FILE VALUES
    if (logToMon)
    {
        printConfigValues(settings, fileName);
    }
    
    if (logToFile)
    {
        appendSettingsToLog(logList, settings);
    }
    
    //BEGINNING MDF FILE UPLOAD
    runTime = execTime(startTime);
    sprintf(logDescription, "[%lf]     Begin %s file upload...\n\n", runTime, settings->mdfPath);
    printf("%s", logDescription);
    appendToLog(logList, logDescription);
	
    mdfVal = readMetaDataFile(settings->mdfPath, actionsHead);
	
    //ERROR CHECKING FOR META-DATA FILE READING
    if (mdfVal > 0) 
    {
        if (logToFile)
        {
            createLogFile(settings->logPath, logList);
        }
        displayError(mdfVal);
    }

    if (logToMon)
    {
        printSimActions(actionsHead, settings);
    }

    if (logToFile)
    {
        appendSimActionsToLog(logList, actionsHead);
    }
    
    //PRINTING SUCCESS MESSAGE
    runTime = execTime(startTime);
    sprintf(logDescription, "[%lf]     %s uploaded succesfully!\n\n", runTime, settings->mdfPath);
    
    //PRINT TO LOGIC
    if (logToMon)
    {
        printSimActions(actionsHead, settings);
    }

    if (logToFile)
    {
        appendSimActionsToLog(logList, actionsHead);
    }

    verificationVal = verifySimActions(actionsHead);

    if (verificationVal > 0)
    {
        if (logToFile)
        {
            createLogFile(settings->logPath, logList);
        }

        displayError(verificationVal);
    }

    numApps = countApplications(actionsHead);

    pcbList = (struct PCB **) malloc(sizeof(struct PCB) * numApps);

    createPCBList(pcbList, actionsHead);

    simVal = simulate(pcbList, settings);

    printf("%d\n", simVal);

    //FREEING DATA STRUCTS USED TO STORE READ INFORMATION
    freeActions(actionsHead);
    free(settings);

    return 0;
}
