#ifndef BITSET_H
#define BITSET_H

#include <stdint.h>

typedef uint8_t* bitset;

void writeBit(bitset array, int bitIndex, int value);
uint8_t readBit(bitset array, int bitIndex);
uint8_t readBitLog(bitset array, int bitIndex);
void clearBitset(bitset array, int length);

#endif