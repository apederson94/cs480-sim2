#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES

//DATA STRUCTURES AND THEIR RELATED FUNCTIONS

//HOLDS CONFIG FILE INFORMATION
struct configValues
{
    float ver;

    char *mdfPath;
    char *cpuSched;
    char *logPath;
    char *logTo;

    int quantumTime;
    int memoryAvailable;
    int cpuCycleTime;
    int ioCycleTime;
};

//HOLDS ALL INFORMATION PERTAINING TO ONE ACTION
struct simAction 
{
    char commandLetter;
    char operationString[20];
    int assocVal;
    struct simAction *next;
};

//FREES ALL MEMORY ASSOCIATED WITH simActionS
void freeActions(struct simAction* head);

//PRINTS ALL RELEVANT simAction INFORMATION
//void printSimAction(struct simAction *src);

//PRINTS ALL ALL RELEVANT simAction INFORMATION FOR ALL simActionS
void printSimActions(struct simAction *head, struct configValues *settings);

//PRINTS ALL INFORMATION FORM A configValues STRUCT
void printConfigValues(struct configValues *src, char *fileName);

//TURNS A COMMAND STRING INTO AN ACTION
int setActionData(char *command, struct simAction *action);

#endif