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

//PRINTS ALL ALL RELEVANT simAction INFORMATION FOR ALL simActionS
void printSimActions(struct simAction *head, struct configValues *settings) 
{
    struct simAction *ptr = head;

    while (ptr->next)
    {
        printf("Op code letter: %c\n", ptr->commandLetter);
        printf("Op code name  : %s\n", ptr->operationString);
        printf("Op code value : %d\n\n", ptr->assocVal);
        ptr = ptr->next;
    }

    free(ptr);
}

//PRINTS ALL INFORMATION FORM A configValues STRUCT
void printConfigValues(struct configValues *src, char *fileName) 
{

    printf("Version                : %f\n", src->ver);
    printf("Program file name      : %s\n", src->mdfPath);
    printf("CPU schedule selection : %s\n", src->cpuSched);
    printf("Quantum time           : %d\n", src->quantumTime);
    printf("Memory Available       : %d\n", src->memoryAvailable);
    printf("Process cycle rate     : %d\n", src->cpuCycleTime);
    printf("I/O cycle rate         : %d\n", src->ioCycleTime);
    printf("Log to selection       : %s\n", src->logTo);
    printf("Log file name          : %s\n\n", src->logPath);
    
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

//RETURNS THE NUMBER OF APPLICATIONS TO BE RUN IN A SIM ACTION LIST
int countApplications(struct simAction *head)
{
    int count = 0;
    struct simAction *tmp = head;

    while (tmp->next)
    {
        if (tmp->commandLetter == 'A' && strCmp(tmp->operationString, "start"))
        {
            count++;
        }
    }

    free(tmp);
    return count;
}

/*verifies that:
    * system:
        * system starts first
        * system starts once
        * system end is the final call
    * application:
        * application must start before ending
        * application must end before starting again
    * returns ERROR_CODE or 0 if successful*/
int verifySimActions(struct simAction *head)
{
    bool osStart = FALSE;
    bool appStart = FALSE;
    bool osEnd = FALSE;
    bool first = TRUE;
    struct simAction *tmp = head;
    char *opString;
    char cmd;

    while (tmp->next)
    {
        cmd = tmp->commandLetter;
        opString = tmp->operationString;

        if (first)
        {
            //first command must be start OS
            if (cmd == 'S'
            && strCmp(opString, "start"))
            {
                osStart = TRUE;
                first = FALSE;
            }
            else
            {
                return OS_START_ERROR;
            }
        }
        else
        {
            //OS may only start once
            if (cmd == 'S'
            && strCmp(opString, "start"))
            {
                return MULTIPLE_OS_START_ERROR;
            }
            else if (!osEnd)
            {
                if (cmd == 'A')
                {
                    if (appStart)
                    {
                        //starting app while another has not ended results in error
                        if (strCmp(opString, "start"))
                        {
                            return CONCURRENT_APP_START_ERROR;
                        }
                        else
                        {
                            appStart = FALSE;
                        }
                    }
                    else
                    {
                        if (strCmp(opString, "start"))
                        {
                            appStart = TRUE;
                        }
                        //if app hasn't started and end is called, result is error
                        else
                        {
                            return APP_END_TIME_ERROR;
                        }
                    }
                }
                else if (cmd == 'S')
                {
                    if (strCmp(opString, "end"))
                    {
                        osEnd = TRUE;
                    }
                }
            }
            //anything coming after OS end call results in error
            else
            {
                return OS_END_ERROR;
            }
            
        }
    }

    free(tmp);
    return 0;
}