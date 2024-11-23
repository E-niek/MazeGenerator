#include "random.h"

unsigned randomIndex = 0;

int randInt(int min, int max, int *seed)
{
    if(randomIndex >= INT32_MAX)
    {
        randomIndex = 0;
    }

    srand(seed == NULL ? time(0) + randomIndex : *seed + randomIndex);
    randomIndex ++;

    return (rand() % (max + 1 - min)) + min;
}
