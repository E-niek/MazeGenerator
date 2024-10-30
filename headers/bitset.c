#include "bitset.h"

bool writeBit(bitset maze, int byte, int bit, int value)
{
    maze[byte] = value == true ? 1 << bit | maze[byte] : ~(1 << bit | ~maze[byte]);

    return true;
}

char readBit(bitset maze, int byte, int bit)
{
    return (1 << bit & maze[byte]) == 1 << bit ? 1 : 0;
}
