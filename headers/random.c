#include "random.h"

int randomIndex = 0;

int randInt(int min, int max, int *seed)
{
    srand(seed == NULL ? time(0) + randomIndex : *seed);
    randomIndex ++;

    return (rand() % (max + 1 - min)) + min;
}
