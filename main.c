#include <stdio.h>
#include <stdlib.h>
#include "booleans.h"
#include "strUtils.h"
#include "fileUtils.h"
#include "dataStructures.h"
#include "errors.h"

int main(int argc, char const *argv[]) {

    //VARIABLE DECLARATIONS
    char *fileExt;
    char *fileName = (char*) argv[1];
    struct configValues *settings = (struct configValues*) malloc(sizeof(struct configValues));
    struct simAction *actionsHead = (struct simAction*) malloc(sizeof(struct simAction));
    int cfgVal, mdfVal;
    bool logToFile = FALSE;
    bool logToMon = FALSE;
    
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
        displayError(cfgVal);
        return 1;
    }

    //PRINTING SUCCESS MESSAGE
    printf("%s uploaded succesfully!\n\n", fileName);

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
