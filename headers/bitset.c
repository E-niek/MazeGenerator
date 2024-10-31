#include "bitset.h"

void writeBit(bitset maze, int byte, int bit, int value)
{
    maze[byte] = value == 0 ? ~(1 << bit | ~maze[byte]) : 1 << bit | maze[byte] ;
}

uint8_t readBit(bitset maze, int byte, int bit)
{
    return (1 << bit & maze[byte]) == 1 << bit ? 1 : 0;
}
