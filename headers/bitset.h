#ifndef BITSET_H
#define BITSET_H

#include "utilities.h"

typedef unsigned char bitset[];

bool writeBit(bitset maze, int byte, int bit, int value);
char readBit(bitset maze, int byte, int bit);

#endif