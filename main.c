#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "booleans.h"
#include "strUtils.h"
#include "fileUtils.h"
#include "dataStructures.h"
#include "errors.h"
#include "logger.h"

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
    struct PCB *pcbList;
    int cfgVal, mdfVal;
    clock_t startTime, currentTime;
    bool logToFile = FALSE;
    bool logToMon = FALSE;

    startTime = clock();
    
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
    currentTime = execTime(startTime);
    sprintf(logDescription, "[%lf]     Begin %s upload...\n\n", currentTime, fileName);
    printf(logDescription);
    loggedOps(logList, currentTime, 0, logDescription);




	//READ IN CONFIG FILE VALUES
    cfgVal = readConfigFile(fileName, settings);

    //ERROR CHECKING FOR CONFIG FILE READING
    if (cfgVal > 0) 
    {
        if (logToFile)
        {
            createLogFile(settings->logPath, logList);
        }
        displayError(cfgVal);
        return 1;
    }

    //PRINTING SUCCESS MESSAGE
    currentTime = execTime(startTime);
    sprintf(logDescription, "[%lf]     %s uploaded successfully!\n\n", currentTime, fileName);
    printf(logDescription);
    logOp(logList, currentTime, 0, logDescription);

    //PRINTING CONFIG FILE VALUES
    printConfigValues(settings, fileName);

    //BEGINNING MDF FILE UPLOAD
    printf("Begin %s file upload...\n\n", settings->mdfPath);
    //printf("Meta-Data File Display\n======================\n\n");

	
    mdfVal = readMetaDataFile(settings->mdfPath, actionsHead);
	
    //ERROR CHECKING FOR META-DATA FILE READING
    if (mdfVal > 0) 
    {
        displayError(mdfVal);
    }
    
    //PRINTING SUCCESS MESSAGE
    printf("%s uploaded succesfully!\n\n", settings->mdfPath);
    
    //PRINT TO LOGIC
    printSimActions(actionsHead, settings);



    //FREEING DATA STRUCTS USED TO STORE READ INFORMATION
    freeActions(actionsHead);
    free(settings);

    return 0;
}
