#include "main.h"
#include "bitset.h"
#include "random.h"
#include <tga.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>

enum Direction { LEFT, RIGHT, UP, DOWN };

unsigned short rows;
unsigned short columns;
bitset walls;
bitset visited;

char *fileName;

void printHelp()
{
    printf
    (
        "USAGE: %s [OPTIONS]\n"
        "\n"
        "Available options:\n"
        "   -r: number of rows\n"
        "   -c: number of columns\n"
        "   -s: seed\n"
        "   -o: output; stdout, file, img\n"
        "   -n: if the output is a file or img, specify the name here\n"
        "   -f: input a file to generate the maze from\n"
        "   -h: shows this help"
        "\n", fileName
    );

    exit(0);
}

int main(int argc, char **argv)
{
    int *seed = NULL;
    bool printToStdout = true;
    bool saveAsMaze = false;
    char saveMazeName[100] = "maze";
    bool saveAsImg = false;
    char saveImgName[100] = "maze";
    bool loadFromMaze = false;
    char loadMazeName[100];
    
    fileName = argv[0];

    if(argc == 1)
    {
        printHelp();
    }

    int option = 0;
    bool noOption = true;
    while((option = getopt(argc, argv, "r:c:s:o:n:f:h")) != -1)
    {
        noOption = false;

        switch(option)
        {
            case 'r':
                rows = atoi(optarg);
                break;
            case 'c':
                columns = atoi(optarg);
                break;
            case 's':
                int seedValue = atoi(optarg);
                seed = &seedValue;
                break;
            case 'o':
                printToStdout = strstr(optarg, "stdout");
                saveAsMaze = strstr(optarg, "file");
                saveAsImg = strstr(optarg, "img");
                break;
            case 'n':
                if(strlen(optarg) > 100)
                {
                    fprintf(stderr, "ERROR: Filename exceeds max length of 100\n");
                    exit(1);
                }
                strcpy(saveMazeName, optarg);
                strcpy(saveImgName, optarg);
                break;
            case 'f':
                loadFromMaze = true;
                if(strlen(optarg) > 100)
                {
                    fprintf(stderr, "ERROR: Filename exceeds max length of 100\n");
                    exit(1);
                }
                strcpy(loadMazeName, optarg);
                break;
            case 'h':
            default:
                printHelp();
                break;
        }
    }

    if(noOption)
    {
        printHelp();
    }

    setSeed(seed);

    unsigned int wallsLength;

    if(loadFromMaze)
    {
        loadMaze(loadMazeName);
    }else
    {
        wallsLength = allocateMemory(true);
        generateMaze(0, 0);
    }

    if(printToStdout)
    {
        printStdout();
    }
    if(saveAsMaze)
    {
        saveMaze(wallsLength, saveMazeName);
    }
    if(saveAsImg)
    {
        saveImg(saveImgName);
    }

    free(walls);
    free(visited);

    return 0;
}

unsigned long long allocateMemory(bool allocateVisited)
{
    unsigned int wallsBitcount = 2 * (rows * columns) - rows - columns;
    unsigned int wallsBytecount = (wallsBitcount + 7) / 8;
    walls = malloc(wallsBytecount);
    clearBitset(walls, wallsBytecount);

    if(allocateVisited)
    {
        unsigned long long visitedBitcount = rows * columns;
        unsigned long long visitedBytecount = (visitedBitcount + 7) / 8;
        visited = malloc(visitedBytecount);
        // we don't need to use clearBitset() because default value already is 0

        // we set the first bit to 1, because else the first cell is 'never visited'
        writeBit(visited, 0, 1);
    }

    // return the length of the walls array, for if we save the maze
    return wallsBytecount;
}

void generateMaze(unsigned int row, unsigned int column)
{
    unsigned short currentRow = row;
    unsigned short currentColumn = column;

    uint8_t possibleDirections = 0;
    uint8_t directions[4];

    for(int i = 0; i < 4; i++)
    {
        if(possibleDirection(currentRow, currentColumn, i) == true)
        {
            directions[possibleDirections] = i;
            possibleDirections++;
        }
    }

    if(possibleDirections == 0)
    {
        return;
    }

    switch(directions[randInt(0, possibleDirections - 1)])
    {
        case LEFT:
            // 'break' the left wall
            writeBit(walls, 2 * (row * columns) - row + column - 1, 0);
            column--;
            break;
        case RIGHT:
            // 'break' the right wall
            writeBit(walls, 2 * (row * columns) - row + column, 0);
            column++;
            break;
        case UP:
            // 'break' the top wall
            writeBit(walls, 2 * (row * columns) - row - columns + column, 0);
            row--;
            break;
        case DOWN:
            // 'break' the bottom wall
            writeBit(walls, 2 * (row * columns) - row + columns + column - 1, 0);
            row++;
            break;
    }
    
    writeBit(visited, row * columns + column, 1);
    generateMaze(row, column);
    
    // we substract one, because we already did go to one of the possible directions above
    for(int i = 0; i < possibleDirections - 1; i++)
    {
        generateMaze(currentRow, currentColumn);
    }
}

int possibleDirection(unsigned int row, unsigned int column, uint8_t side)
{
    switch(side)
    {
        case LEFT:
            // calculate which bit is left to the row and column we are in
            return column > 0 && readBit(walls, 2 * row * columns - row + column - 1) == 1 && readBit(visited, row * columns + column - 1) == 0 ? true : false;
            break;
        case RIGHT:
            // calculate which bit is right to the row and column we are in
            return column < columns - 1 && readBit(walls, 2 * row * columns - row + column) == 1 && readBit(visited, row * columns + column + 1) == 0 ? true : false;
            break;
        case UP:
            // calculate which bit is up to the row and column we are in
            return row > 0 && readBit(walls, 2 * row * columns - row - columns + column) == 1 && readBit(visited, row * columns + column - columns) == 0 ? true : false;
            break;
        case DOWN:
            // calculate which bit is down to the row and column we are in
            return row < rows - 1 && readBit(walls, 2 * row * columns - row + columns + column - 1) == 1 && readBit(visited, row * columns + column + columns) == 0 ? true : false;
            break;
    }
    return false;
}

void loadMaze(char *loadMazeName)
{
    FILE *file = fopen(loadMazeName, "r");

    if(file == NULL)
    {
        fprintf(stderr, "%s: cannot open file -- '%s'\n", fileName, loadMazeName);
        printHelp();
    }

    char buffer[6];

    int row = 0;
    long long wallsIndex = 0;
    while(fgets(buffer, 6, file))
    {
        switch(row)
        {
            case 0:
                rows = atoi(buffer);
                break;
            case 1:
                columns = atoi(buffer);
                break;
            case 2:
                allocateMemory(false);
                // we don't break here, because after allocation we still need to store then value of row 2 in walls
            default:
                walls[wallsIndex] = atoi(buffer);
                wallsIndex++;
                break;
        }
        row++;
    }

    fclose(file);
}

void printStdout()
{
    char wall = '#';
    char path = ' ';

    for(unsigned short row = 0; row < rows; row++)
    {
        for(unsigned short column = 0; column < columns; column++)
        {
            // check the wall index above the current row and column
            printf("%c %c ", wall, row > 0 && readBit(walls, 2 * row * columns - row - columns + column) == 0 ? path : wall);
        }
        printf("%c\n", wall);

        for(unsigned short column = 0; column < columns; column++)
        {
            // check the wall index left to the current row and column
            printf("%c %c ", column > 0 && readBit(walls, 2 * row * columns - row + column - 1) == 0 ? path : wall, path);
        }
        printf("%c\n", wall);
    }

    for(unsigned short column = 0; column < columns; column++)
    {
            printf("%c %c ", wall, wall);
    }
    printf("%c", wall);
}

void saveMaze(unsigned int wallsLength, char *saveMazeName)
{
    char *mazeNameBuffer = saveMazeName;

    FILE *file = fopen(strcat(mazeNameBuffer, ".maze"), "w");
    fprintf(file, "%d\n", rows);
    fprintf(file, "%d\n", columns);

    for(unsigned long long i = 0; i < wallsLength; i++)
        {
        fprintf(file, "%d\n", walls[i]);
    }

    fclose(file);
}

void saveImg(char *saveImgName)
{
    char *imgNameBuffer = saveImgName;

    unsigned int width = columns * 2 + 1;
    unsigned int height = rows * 2 + 1;

    uint8_t dataBGRA[width * height];

    int wall = 0;
    int path = UINT8_MAX;

    for(unsigned short row = 0; row < rows; row++)
    {
        for(unsigned short column = 0; column < columns; column++)
        {
            int index = 2 * row * width + column * 2;
            dataBGRA[index] = wall;
            // check the wall index above the current row and column
            dataBGRA[index + 1] = row > 0 && readBit(walls, 2 * row * columns - row - columns + column) == 0 ? path : wall;

        }
        dataBGRA[2 * row * width + width - 1] = wall;

        for(unsigned short column = 0; column < columns; column++)
        {
            int index = 2 * row * width + width + column * 2;
            // check the wall index left to the current row and column
            dataBGRA[index] = column > 0 && readBit(walls, 2 * row * columns - row + column - 1) == 0 ? path : wall;
            dataBGRA[index + 1] = path;
        }
        dataBGRA[2 * row * width + width * 2 - 1] = wall;
    }

    for(unsigned short column = 0; column < columns; column++)
    {
        int index = 2 * rows * width + column * 2;
        dataBGRA[index] = wall;
        dataBGRA[index + 1] = wall;
    }
    dataBGRA[width * height - 1] = wall;

    writeImage(strcat(imgNameBuffer, ".tga"), width, height, dataBGRA, 1, 3);
}
