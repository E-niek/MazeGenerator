#include "headers/main.h"

enum DIRECTION { LEFT, RIGHT, UP, DOWN };

int rows = 20;
int columns = 20;
bitset walls;
bitset visited;
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

    // 0 of there is no wall, 1 if there is
    int wallsBitCount = 2 * (rows * columns) - rows - columns;
    int wallsByteCount = (wallsBitCount + 7) / 8;
    walls = malloc(wallsByteCount);
    clearBitset(walls, wallsByteCount);

    // 0 if cell isn't visited, 1 if it is
    int visitedBitCount = rows * columns;
    int visitedByteCount = (visitedBitCount + 7) / 8;
    visited = malloc(visitedByteCount);
    // we don't need to use clearBitset() because default value already is 0

    generateMaze(0, 0, 0, 0, 0);

    free(walls);
    free(visited);

    return 0;
}

void generateMaze(int row, int column, int lastRow, int lastColumn, int run)
{
    int index = 0;
    int directions[4];

    for(int i = 0; i < 4; i++)
    {
        if(possibleDirection(row, column, i))
        {
            directions[index] = i;
            index ++;
        }
    }
    for(int i = 0; i < index; i++)
    {
        printf("Possible direction: %d\n", directions[i]);
    }

    if(index > 0)
    {
        if(index > 1)
        {
            lastRow = row;
            lastColumn = column;
        }
        
        switch(directions[randomInt(0, index - 1, seed)])
        {
            case LEFT:
                printf("LEFT\n");
                writeBit(walls, 2 * (row * columns) - row + column, 0);
                writeBit(visited, row * columns + column, 1);
                generateMaze(row, column - 1, lastRow, lastColumn, run + 1);
                break;
            case RIGHT:
                printf("RIGHT\n");
                writeBit(walls, 2 * (row * columns) - row + column + 1, 0);
                writeBit(visited, row * columns + column, 1);
                generateMaze(row, column + 1, lastRow, lastColumn, run + 1);
                break;
            case UP:
                printf("UP\n");
                writeBit(walls, 2 * (row * columns) - row - columns + column + 1, 0);
                writeBit(visited, row * columns + column, 1);
                generateMaze(row - 1, column, lastRow, lastColumn, run + 1);
                break;
            case DOWN:
                printf("DOWN\n");
                writeBit(walls, 2 * (row * columns) - row + columns + column, 0);
                writeBit(visited, row * columns + column, 1);
                generateMaze(row + 1, column, lastRow, lastColumn, run + 1);
                break;
        }
    }else if(run < rows * columns - 1)
    {
        return;
        // printf("GOING BACK\n");
        // generateMaze(lastRow, lastColumn, lastRow, lastColumn, run + 1);
    }else
    {
        printMaze();
        // printf("Stopped\n");
    }
}

int possibleDirection(int row, int column, int side)
{
    switch(side)
    {
        case LEFT:
            // calculate which bit is left to the row and column we are in
            return readBit(walls, 2 * (row * columns) - row + column) == 1 && readBit(visited, row * columns + column - 1) == 0 && column > 0 ? true : false;
            break;
        case RIGHT:
            // calculate which bit is right to the row and column we are in
            return readBit(walls, 2 * (row * columns) - row + column + 1) == 1 && readBit(visited, row * columns + column + 1) == 0 && column < columns - 1 ? true : false;
            break;
        case UP:
            // calculate which bit is up to the row and column we are in
            return readBit(walls, 2 * (row * columns) - row - columns + column + 1) == 1 && readBit(visited, row * columns + column - columns) == 0 && row > 0 ? true : false;
            break;
        case DOWN:
            // calculate which bit is down to the row and column we are in
            return readBit(walls, 2 * (row * columns) - row + columns + column) == 1 && readBit(visited, row * columns + column + columns) == 0 && row < rows - 1 ? true : false;
            break;
    }
    return false;
}

void printMaze()
{
    char wall = '#';
    char path = ' ';
    
    int wallIndex = 0;
    for(int row = 0; row < rows * 2 + 1; row++)
    {
        for(int column = 0; column < columns; column++)
        {
            printf("%c %c ", wall, wall);
        }
        printf("%c\n", wall);
    }
}

/*

# # # # # # #
#       #   #
# # #   #   #
#   #       #
#   #   #   #
#       #   #
# # # # # # #


*/