#include "bitset.h"

void writeBit(bitset maze, int byte, int bit, int value)
{
    maze[byte] = value == 0 ? ~(1 << bit | ~maze[byte]) : 1 << bit | maze[byte] ;
}

uint8_t readBit(bitset maze, int bitIndex)
{
    // add 3 * sizeof(int) because of the first 3 integers in the array (rows, columns, seed)
    int byteIndex = (bitIndex + 7) / 8 + 3 * sizeof(int);
    int bitOffset = bitIndex % 8;
    return (1 << bitOffset & maze[byteIndex]) == 1 << bitOffset ? 1 : 0;
}

void clearBitset(bitset maze, int length)
{
    for(int i = 3 * sizeof(int) + 1; i < length + 1; i++)
    {
        maze[i] = UINT8_MAX;
    }
}
