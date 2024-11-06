#include "bitset.h"

void writeBit(bitset array, int bitIndex, int value)
{
    // add 3 * sizeof(int) because of the first 3 integers in the array (rows, columns, seed)
    int byteIndex = (bitIndex + 7) / 8;
    int bitOffset = bitIndex % 8;
    array[byteIndex] = value == 0 ? ~(1 << bitOffset | ~array[byteIndex]) : 1 << bitOffset | array[byteIndex] ;
}

uint8_t readBit(bitset array, int bitIndex)
{
    // add 3 * sizeof(int) because of the first 3 integers in the array (rows, columns, seed)
    int byteIndex = (bitIndex + 7) / 8;
    int bitOffset = bitIndex % 8;
    return (1 << bitOffset & array[byteIndex]) == 1 << bitOffset ? 1 : 0;
}

void clearBitset(bitset maze, int length)
{
    for(int i = 0; i < length + 1; i++)
    {
        maze[i] = UINT8_MAX;
    }
}
