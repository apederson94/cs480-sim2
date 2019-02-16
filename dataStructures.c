#include "dataStructures.h"
#include "booleans.h"
#include "errors.h"
#include "strUtils.h"
#include <stdio.h>
#include <stdlib.h>

//DATA STRUCTURE FUNCTIONS

//FREES ALL MEMORY ASSOCIATED WITH simActionS
void freeActions(struct simAction* head) 
{
    struct simAction *tmp;

    while (head) 
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

//PRINTS ALL RELEVANT simAction INFORMATION
//CURRENTLY DEPRECATED
/* void printSimAction(struct simAction *src, char *logFile, char *logTo) 
{
    if (strCmp(logTo, "Monitor")
    || strCmp(logTo, "Both")) 
    {
        printf("Op code letter: %c\n", src->commandLetter);
        printf("Op code name  : %s\n", src->operationString);
        printf("Op code value : %d\n", src->assocVal);
    }

    if (strCmp(logTo, "File")
    || strCmp(logTo, "Both"))
    {
        file =
        fprintf("Op code letter: %c\n", src->commandLetter);
        fprintf("Op code name  : %s\n", src->operationString);
        fprintf("Op code value : %d\n", src->assocVal);
    }
    
} */

//PRINTS ALL ALL RELEVANT simAction INFORMATION FOR ALL simActionS
void printSimActions(struct simAction *head, struct configValues *settings) 
{
    char *logPath = settings->logPath;
    char *logTo = settings->logTo;
    FILE *logFile;
    struct simAction *ptr = head;

    if (strCmp(logTo, "File")
    || strCmp(logTo, "Both"))
    {
       logFile = fopen(logPath, "a");
       while (ptr->next) 
        {
            fprintf(logFile, "Op code letter: %c\n", ptr->commandLetter);
            fprintf(logFile, "Op code name  : %s\n", ptr->operationString);
            fprintf(logFile, "Op code value : %d\n", ptr->assocVal);
            fprintf(logFile, "\n");
            ptr = ptr->next;
        }
        fclose(logFile);
    }

    ptr = head;

    if (strCmp(logTo, "Monitor")
    || strCmp(logTo, "Both"))
    {
        while (ptr->next)
        {
            printf("Op code letter: %c\n", ptr->commandLetter);
            printf("Op code name  : %s\n", ptr->operationString);
            printf("Op code value : %d\n", ptr->assocVal);
            printf("\n");
            ptr = ptr->next;
        }
        
    }
    
    free(logFile);
    free(logTo);
    free(ptr);
}

//PRINTS ALL INFORMATION FORM A configValues STRUCT
void printConfigValues(struct configValues *src, char *fileName) 
{
    FILE *logFile;
    char *printTo = src->logTo;

    if (strCmp(printTo, "Monitor")
    || strCmp(printTo, "Both"))
    {
        printf("Version                : %f\n", src->ver);
        printf("Program file name      : %s\n", src->mdfPath);
        printf("CPU schedule selection : %s\n", src->cpuSched);
        printf("Quantum time           : %d\n", src->quantumTime);
        printf("Memory Available       : %d\n", src->memoryAvailable);
        printf("Process cycle rate     : %d\n", src->cpuCycleTime);
        printf("I/O cycle rate         : %d\n", src->ioCycleTime);
        printf("Log to selection       : %s\n", src->logTo);
        printf("Log file name          : %s\n", src->logPath);
        printf("\n");
    }

    if (strCmp(printTo, "File")
    || strCmp(printTo, "Both")) 
    {
        logFile = fopen(src->logPath, "a");
        fprintf(logFile, "Begin %s upload...\n", fileName);
        fprintf(logFile, "%s uploaded sucessfully!\n", fileName);
        fprintf(logFile, "Version                : %g\n", src->ver);
        fprintf(logFile, "Program file name      : %s\n", src->mdfPath);
        fprintf(logFile, "CPU schedule selection : %s\n", src->cpuSched);
        fprintf(logFile, "Quantum time           : %d\n", src->quantumTime);
        fprintf(logFile, "Memory Available       : %d\n", src->memoryAvailable);
        fprintf(logFile, "Process cycle rate     : %d\n", src->cpuCycleTime);
        fprintf(logFile, "I/O cycle rate         : %d\n", src->ioCycleTime);
        fprintf(logFile, "Log to selection       : %s\n", src->logTo);
        fprintf(logFile, "Log file name          : %s\n", src->logPath);
        fclose(logFile);
    }
    
}

//TURNS A COMMAND STRING INTO AN ACTION
int setActionData(char *command, struct simAction *action) 
{
    int cmdLength, pos, opFlag, opIter, value;
    char currentChar, opStr[40];

    cmdLength = strLen(command);
    pos = 0;
    value = 0;
    opIter = 0;
    opFlag = FALSE;
    currentChar = command[pos];

    if (!strContains(command, "(") 
    || !strContains(command, ")")
    || !strContains(command, ";")) 
    {
        return CORRUPTED_MDF_ERROR;
    }

    while (pos < cmdLength) 
    {
        if (!(currentChar == ' ') 
        && charIsUpper(currentChar)) 
        {
            action->commandLetter = command[pos];

        } 
        else if (currentChar == '(') 
        {
            opFlag = TRUE;

        } 
        else if (currentChar == ')') 
        {
            opStr[opIter] = '\0';

            if ((!strCmp(opStr, "start") 
            && !strCmp(opStr, "end")
            && (action->commandLetter == 'S' || action->commandLetter == 'A'))) 
            {
                return SA_OP_STRING_ERROR;

            } 
            else if (!strCmp(opStr, "run") && action->commandLetter == 'P') 
            {
                return P_OP_STRING_ERROR;

            } 
            else if (!strCmp(opStr, "hard drive")
            && !strCmp(opStr, "keyboard")
            && action->commandLetter == 'I') 
            {
                return I_OP_STRING_ERROR;
                
            } 
            else if (!strCmp(opStr, "hard drive") 
            && !strCmp(opStr, "printer")
            && !strCmp(opStr, "monitor") 
            && action->commandLetter == 'O') 
            {
                return O_OP_STRING_ERROR;

            } 
            else if (!strCmp(opStr, "allocate") 
            && !strCmp(opStr, "access") 
            && action->commandLetter == 'M') 
            {
                return M_OP_STRING_ERROR;
            }
            strCopy(opStr, action->operationString);
            opFlag = FALSE;
        } 
        else if (opFlag) 
        {
            opStr[opIter] = currentChar;
            opIter++;
        } 
        else if (!opFlag 
        && !(currentChar == ';') 
        && charIsNum(currentChar)) 
        {
            value *= 10;
            value += (currentChar - 48);
        }
        pos++;
        currentChar = command[pos];
    }
    if (value < 0) {
        return NEGATIVE_MDF_VALUE_ERROR;
    }
    action->assocVal = value;
    return 0;
}