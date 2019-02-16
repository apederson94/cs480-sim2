#include "booleans.h"
#include "mathUtils.h"
#include "errors.h"
#include <stdlib.h>

//ALL STRING-RELATED UTILITIES

//RETURNS STRING LENGTH
int strLen(const char* string) 
{
    char curr = string[0];
    int len = 0;
    while (curr != '\0') 
    {
        len += 1;
        curr = string[len];
    }

    return len;
}

//COPIES ONE STRING INTO ANOTHER
void strCopy(char *src, char *dest) 
{
    int len = strLen(src);
    int pos;
    if (src && dest) 
    {
        for (pos = 0; pos < len; pos++) 
        {
            dest[pos] = src[pos];
        }
        dest[pos] = '\0';
    }
}

//READS THE LAST 4 OF A STRING TO DETERMINE THE EXTENSION
char* getFileExt(char* src) 
{
    int len = strLen(src);
    char *ext = (char*) malloc(sizeof(char) * 5);
    ext[4] = '\0';

    for (int pos = 4; pos > 0; pos--) 
    {
        ext[4-pos] = src[len-pos];
    }

    return ext;
}

//COMPARES TWO STRINGS
int strCmp(const char* src, char* target) 
{
    int src_len = strLen(src);

    if (src_len != strLen(target)) 
    {
        return FALSE;
    }

    for (int pos = 0; pos < src_len; pos++) 
    {
        if (src[pos] != target[pos])
        {
            return FALSE;
        }
    }

    return TRUE;
}

//CHECKS TO SEE IF STRING HAS SUBSTRING
int strContains(char* src, char* substr) 
{
    int srcIter = 0;
    int substrIter = 0;
    char srcCurr = src[0];
    char substrChar = substr[0];

    while (srcCurr && substrChar) 
    {
        srcCurr = src[srcIter];

        if (srcCurr == substrChar) 
        {
            substrIter++;
            substrChar = substr[substrIter];
            srcIter++;

        } 
        else 
        {
            if (substrIter == 0) 
            {
                srcIter++;
            }

            substrIter = 0;
            substrChar = substr[0];
        }
    
    }

    //IF substrChar = NULL, RETURN TRUE
    if (!substrChar) 
    {
        return TRUE;
    }

    return FALSE;
}

//RETURNS END SYMBOL POSITION OF SUBSTRING IN STRING
int substrPos(char *src, char *target) 
{
    int srcIter = 0;
    int targetIter = 0;
    char srcChar = src[0];
    char targetChar = target[0];

    if (!strContains(src, target)) 
    {
        return -1;
    }

    while (srcChar && targetChar) 
    {
        srcChar = src[srcIter];
        
        if (srcChar == targetChar) 
        {
            targetIter++;
            targetChar = target[targetIter];
            srcIter++;

        } 
        else 
        {
            if (targetIter == 0) 
            {
                srcIter++;
            }

            targetIter = 0;
            targetChar = target[0];
        }
    }

    return srcIter;
}

//REVERSES A STRING IN PLACE
void rev_str(char *src) 
{
    char tmp;
    int len = strLen(src) - 1; //IGNORE NULL TERMINATOR
    int halfLen = len / 2;

    if (len % 2 != 0) 
    {
        halfLen++;
    }

    for (int pos = 0; pos < halfLen; pos++) 
    {
        tmp = src[len - pos];
        src[len - pos] = src[pos];
        src[pos] = tmp;
    }
}

//CREATES A SUBSTRING FROM INDEX START TO END AND STORES IT IN substr
void substr(char *src, int start, int end, char *substr) 
{
    int iter = 0;

    for (int pos = start; pos < end; pos++) 
    {
        substr[iter] = src[pos];
        iter++;
    }

    substr[iter] = '\0';
}

//REMOVES ALL NON_SYMBOL CHARACTERS
void removeNonSymbols(char *src) 
{
    int len = strLen(src);
    int tmpPos = 0;
    int pos;
    char sym;
    char *tmp = (char*) malloc(sizeof(char) * 100);

    for (pos = 0; pos < len; pos++) 
    {
        sym = src[pos];

        if ((sym >= 'A' && sym <= 'Z') 
        || (sym >= 'a' && sym <= 'z') 
        || (sym >= '0' && sym <= '9')
        || sym == '-' 
        || sym == '.') 
        {
            tmp[tmpPos] = sym;
            tmpPos++;
        }
    }

    tmp[pos] = '\0';
    strCopy(tmp, src);
    free(tmp);
}

//EVALUATES A SINGLE CHAR TO AN INT
int c2i(char src)
{
    int num;

    switch (src)
    {
        case '0':
            num = 0;
            break;
        case '1':
            num = 1;
            break;
        case '2':
            num = 2;
            break;
        case '3':
            num = 3;
            break;
        case '4':
            num = 4;
            break;
        case '5':
            num = 5;
            break;
        case '6':
            num = 6;
            break;
        case '7':
            num = 7;
            break;
        case '8':
            num = 8;
            break;
        case '9':
            num = 9;
            break;
        default:
            num = -1;
    }
    
    return num;
}

//CONVERTS A STRING TO A FLOAT VALUE
float s2f(char *src) 
{
    int iter = 0;
    char currChar = src[0];
    int decimalPos = substrPos(src, ".");
    int places = decimalPos - 2; //SUBTRACT 2 IN ORDER TO GET TO ONE BEFORE THE TARGET CHAR
    float num = 0.0f;

    while (currChar) 
    {
        if (currChar >= '0' && currChar <= '9') 
        {
            num += c2i(currChar) * raiseToPower(10, places);
            places--;
        } else if (currChar == '.') {
            places = -1;
        }

        iter++;
        currChar = src[iter];
        
    }

    return num;
}

//converts a string into an int value
int s2i(char *src) 
{
    int len = strLen(src);
    int num = 0;
    int currNum;
    for (int pos = 0; pos < len; pos++) 
    {
        num *= 10;
        currNum = c2i(src[pos]);

        if (currNum >= 0 && currNum < 10) 
        {
            num += currNum;
        } 
        else 
        {
            return currNum;
        }
    }

    return num;
}

//CHECKS PROVIDED STRING AGAINST COMPATIBLE SCHEDULER TYPES
int checkCpuSched(char *src) 
{
    return strCmp(src, "FCFS-N")
    || strCmp(src, "SJF-N")
    || strCmp(src, "SRTF-P")
    || strCmp(src, "FCFS-P") 
    || strCmp(src, "RR-P");
}

//CHECKS STRING AGAINST COMPATIBLE LOG TO TYPES
int checkLogTo(char *src) 
{
    return strCmp(src, "Monitor") 
    || strCmp(src, "File") 
    || strCmp(src, "Both");
}

//SETTS ALL VALUES IN A STRING TO NULL
void strClear(char *src) 
{
    int len = strLen(src);

    for (int pos = len; pos > 0; pos--) 
    {
        src[pos] = '\0';
    }
}

//REMOVES ALL NON-NUMBERS FROM A STRING
void strRmNonNumbers(char *src) 
{
    int pos, len, num;

    pos = 0;
    len = strLen(src);

    for (pos = 0; pos < len; pos++) 
    {
        num = c2i(src[pos]);

        if (num > 9 || num < 0) 
        {
            src[pos] = src[pos+1];
        }
    }
}

//CHECKS IF A CHARACTER IS A NUMBER
int charIsNum(char src) 
{
    return src >= '0' && src <= 58;
}

//CHECKS IF A CHARACTER IS UPPERCASE
int charIsUpper(char src) 
{
    return src >= 'A' && src <= 'Z';
}