#ifndef BITSET_H
#define BITSET_H

#include "utilities.h"

typedef uint8_t* bitset;

void writeBit(bitset maze, int byte, int bit, int value);
uint8_t readBit(bitset maze, int bitIndex);
void clearBitset(bitset maze, int length);

#endif