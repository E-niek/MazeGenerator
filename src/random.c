#include "random.h"
#include <stdlib.h>
#include <time.h>

void setSeed(int *seed)
{
    srand(seed == NULL ? time(0): *seed);
}

int randInt(int min, int max)
{
    return (rand() % (max + 1 - min)) + min;
}
