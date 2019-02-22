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
    struct configValues *settings = (struct configValues*) calloc(1, sizeof(struct configValues));
    struct simAction *actionsHead = (struct simAction*) calloc(1, sizeof(struct simAction));
    struct logEntry *logList = (struct logEntry*) calloc(1, sizeof(struct logEntry));
    int cfgVal, mdfVal, verificationVal, simVal;
    bool logToFile = FALSE;
    bool logToMon = FALSE;

    printf("===================\nSIMULATOR PROGRAM\n===================\n\n");

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

    free(fileExt);

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
        createLogHeader(logList);
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

    //PRINTING SUCCESS MESSAGE
    printf("%s uploaded succesfully!\n\n", settings->mdfPath);

    if (logToMon)
    {
        printSimActions(actionsHead, settings);
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

    simVal = simulate(actionsHead, settings, logList);

    if (simVal) {
        //TODO: ERRORS???
    }

    if (logToFile)
    {
        createLogFile(settings->logPath, logList);
    }

    //FREEING DATA STRUCTS USED TO STORE READ INFORMATION
    
    freeActions(actionsHead);
    freeLog(logList);
    freeConfigValues(settings);

    return 0;
}
