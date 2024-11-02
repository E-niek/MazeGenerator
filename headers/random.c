#include "random.h"

int randomIndex = 0;

int randomInt(int min, int max, int *seed)
{
    srand(seed == NULL ? time(0) + randomIndex : *seed);
    randomIndex ++;

    return (rand() % (max + 1 - min)) + min;
}
