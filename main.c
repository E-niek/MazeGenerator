#include "headers/main.h"

enum DIRECTION { LEFT, RIGHT, UP, DOWN };

int rows = 20;
int columns = 20;
int bitCount;
int *seed = NULL;

int main(int argc, char **argv)
{
    for(int i = 1; i < argc; i++)
    {
        rows = strcmp(argv[i], "-r") == 0 ? atoi(argv[i + 1]) : rows;
        columns = strcmp(argv[i], "-c") == 0 ? atoi(argv[i + 1]) : columns;
        if(strcmp(argv[i], "-s") == 0)
        {
            int seedValue = atoi(argv[i + 1]);
            seed = &seedValue;
        }
    }

    bitCount = 2 * (rows * columns) - rows - columns;

    // we add 7 to byteCount to eliminate the effect that int floors a number
    // we add 3 integers to the bitset to store the rowcount, columncount and the seed
    int byteCount = (bitCount + 7) / 8 + 3 * sizeof(int);
    bitset maze = malloc(byteCount);

    clearBitset(maze, byteCount);

    printf("Possible direction: %d\n", possibleDirection(maze, 1, 0, UP));

    free(maze);
}

void generateMaze(bitset maze, int row, int column, int lastRow, int lastColumn)
{
    
}

int possibleDirection(bitset maze, int row, int column, int side)
{
    switch(side)
    {
        case LEFT:
            // calculate which bit is left to the row and column we are in
            return readBit(maze, 2 * (row * columns) - row + column) == 1 && column > 0 ? true : false;
            break;
        case RIGHT:
            // calculate which bit is right to the row and column we are in
            return readBit(maze, 2 * (row * columns) - row + column + 1) == 1 && column < columns - 1 ? true : false;
            break;
        case UP:
            // calculate which bit is up to the row and column we are in
            return readBit(maze, 2 * (row * columns) - row - columns + column + 1) == 1 && row > 0 ? true : false;
            break;
        case DOWN:
            // calculate which bit is down to the row and column we are in
            return readBit(maze, 2 * (row * columns) - row + columns + column) == 1 && row < rows - 1 ? true : false;
            break;
    }
    return false;
}

void printMaze()
{
    char wall = '#';
    char path = ' ';

    for(int row = 0; row < rows * 2 + 1; row++)
    {
        for(int column = 0; column < columns; column++)
        {
            printf("%c %c ", wall, row % 2 == 1 ? path : wall);
        }
        printf(row < rows * 2 ? "%c\n" : "%c", wall);
    }
}

/*

# # # # # # #
#   #   #   #
# # # # # # #
#   #   #   #
# # # # # # #
#   #   #   #
# # # # # # #


*/