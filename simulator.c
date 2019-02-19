#include "dataStructures.h"
#include <stdio.h>

int simulate(struct PCB **pcbList, struct configValues *settings)
{
    int numApps = sizeof(pcbList)/sizeof(struct PCB);
    int pos;
    struct PCB *controlBlock;

    for (pos = 0; pos < numApps; pos++)
    {
        controlBlock = pcbList[pos];

        printf("%c\n", controlBlock->pc->commandLetter);
    }

    return 0;
}