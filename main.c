#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "booleans.h"
#include "dataStructures.h"
#include "errors.h"
#include "fileUtils.h"
#include "logger.h"
#include "simulator.h"
#include "strUtils.h"
#include "timer.h"

int main(int argc, char const *argv[]) {

    //VARIABLE DECLARATIONS
    char *fileExt;
    char *fileName = (char*) argv[1];
    struct configValues *settings = (struct configValues*) malloc(sizeof(struct configValues));
    struct simAction *actionsHead = (struct simAction*) malloc(sizeof(struct simAction));
    struct logEntry *logList = (struct logEntry*) malloc(sizeof(struct logEntry));
    int cfgVal, mdfVal, verificationVal, simVal;
    bool logToFile = FALSE;
    bool logToMon = FALSE;

    printf("SIMULATOR PROGRAM\n===================\n\n");

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
    printf("Begin %s upload...\n\n", fileName);

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
    
    if (strCmp(settings->logTo, "Both")
    || strCmp(settings->logTo, "Monitor"))
    {
        logToMon = TRUE;
    }

    //PRINTING SUCCESS MESSAGE
    printf("%s uploaded successfully!\n\n", fileName);

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
    printf("Begin %s file upload...\n\n", settings->mdfPath);
	
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
    printf("%s uploaded succesfully!\n\n", settings->mdfPath);
    
    //PRINT TO LOGIC
    if (logToMon)
    {
        printSimActions(actionsHead, settings);
    }

    if (logToFile)
    {
        appendSimActionsToLog(logList, actionsHead);
    }

    printf("Verifying simulator actions...\n\n");

    verificationVal = verifySimActions(actionsHead);

    if (verificationVal > 0)
    {
        if (logToFile)
        {
            createLogFile(settings->logPath, logList);
        }

        displayError(verificationVal);
    }

    printf("Sim actions verified!\n\n");

    printf("===============\nSimulator Start\n===============\n\n");

    simVal = simulate(actionsHead, settings);

    printf("%d\n", simVal);

    //FREEING DATA STRUCTS USED TO STORE READ INFORMATION
    freeActions(actionsHead);
    free(settings);

    return 0;
}
