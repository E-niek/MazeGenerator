#include "bitset.h"
#include <stdio.h>

void writeBit(bitset array, int bitIndex, int value)
{
    int byteIndex = bitIndex / 8;
    int bitOffset = bitIndex % 8;
    array[byteIndex] = value == 0 ? ~((1 << bitOffset) | ~array[byteIndex]) : 1 << bitOffset | array[byteIndex] ;
}

uint8_t readBit(bitset array, int bitIndex)
{
    int byteIndex = bitIndex / 8;
    int bitOffset = bitIndex % 8;
    return ((1 << bitOffset) & array[byteIndex]) == 1 << bitOffset ? 1 : 0;
}

void clearBitset(bitset maze, int length)
{
    for(int i = 0; i < length; i++)
    {
        maze[i] = UINT8_MAX;
    }
}
