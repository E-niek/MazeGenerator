#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

void setSeed(int *seed);
int randInt(int min, int max);

#endif