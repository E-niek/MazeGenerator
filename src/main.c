#include "main.h"

enum Direction { LEFT, RIGHT, UP, DOWN };

unsigned int rows = 20;
unsigned int columns = 20;
bitset walls;
bitset visited;

int *seed = NULL;
bool printToStdout = true;
bool saveToFile = false;
char *saveFileName = "maze.maze";
bool loadFromFile = false;
char *loadFileName;

char *fileName;

void printHelp()
{
    printf
    (
        "USAGE: %s [OPTIONS]\n"
        "\n"
        "Available options:\n"
        "   -r: " "amount of rows\n"
        "   -c: " "amount of columns\n"
        "   -s: " "seed\n"
        "   -o: " "output; file, stdout\n"
        "   -n: " "if the output is a file, specify the name of the file here\n"
        "   -f: " "input a file to generate the maze from\n"
        "   -h: " "shows this help"
        "\n", fileName
    );
}

int main(int argc, char **argv)
{
    fileName = argv[0];

    if(argc == 1)
    {
        printHelp();
        exit(0);
    }

    int option;
    while((option = getopt(argc, argv, "r:c:s:o:n:f:h")) != -1)
    {
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
                printToStdout = strstr(optarg, "stdout") == NULL ? false : true;
                saveToFile = strstr(optarg, "file") != NULL ? true : false;
                break;
            case 'n':
                saveFileName = optarg;
                break;
            case 'f':
                loadFromFile = true;
                loadFileName = optarg;
                break;
            case 'h':
            case '?':
                printHelp();
                exit(1);
                break;
        }
    }

    unsigned long long wallsLength;

    if(loadFromFile)
    {
        loadMaze();
    }else
    {
        wallsLength = allocateMemory(true);
        generateMaze(0, 0);
    }

    if(printToStdout || loadFromFile)
    {
        printStdout();
    }

    if(saveToFile)
    {
        saveMaze(wallsLength);
    }

    free(walls);
    free(visited);

    return 0;
}

unsigned long long allocateMemory(bool allocateVisited)
{
    // 0 of there is no wall, 1 if there is
    unsigned long long wallsBitcount = 2 * (rows * columns) - rows - columns;
    unsigned long long wallsBytecount = (wallsBitcount + 7) / 8;
    walls = malloc(wallsBytecount);
    clearBitset(walls, wallsBytecount);
    walls[0] = wallsBytecount;

    if(allocateVisited)
    {
        // 0 if cell isn't visited, 1 if it is
        unsigned long long visitedBitcount = rows * columns;
        unsigned long long visitedBytecount = (visitedBitcount + 7) / 8;
        visited = malloc(visitedBytecount);
        // we don't need to use clearBitset() because default value already is 0

        // we set the first bit to 1, because else the first cell is 'never visited'
        writeBit(visited, 0, 1);
    }

    return wallsBytecount;
}

void generateMaze(unsigned int row, unsigned int column)
{
    unsigned int currentRow = row;
    unsigned int currentColumn = column;

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

    switch(directions[randInt(0, possibleDirections - 1, seed)])
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
            return column > 0 && readBit(walls, 2 * (row * columns) - row + column - 1) == 1 && readBit(visited, row * columns + column - 1) == 0 ? true : false;
            break;
        case RIGHT:
            // calculate which bit is right to the row and column we are in
            return column < columns - 1 && readBit(walls, 2 * (row * columns) - row + column) == 1 && readBit(visited, row * columns + column + 1) == 0 ? true : false;
            break;
        case UP:
            // calculate which bit is up to the row and column we are in
            return row > 0 && readBit(walls, 2 * (row * columns) - row - columns + column) == 1 && readBit(visited, row * columns + column - columns) == 0 ? true : false;
            break;
        case DOWN:
            // calculate which bit is down to the row and column we are in
            return row < rows - 1 && readBit(walls, 2 * (row * columns) - row + columns + column - 1) == 1 && readBit(visited, row * columns + column + columns) == 0 ? true : false;
            break;
    }
    return false;
}

void loadMaze()
{
    FILE *file = fopen(loadFileName, "r");

    if(file == NULL)
    {
        fprintf(stderr, "%s: cannot open file -- '%s'\n", fileName, loadFileName);
        printHelp();
        exit(1);
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

    for(unsigned int row = 0; row < rows; row++)
    {
        for(unsigned int column = 0; column < columns; column++)
        {
            // check the wall index above the current row and column
            printf("%c %c ", wall, row > 0 && readBit(walls, 2 * (row * columns) - row - columns + column) == 0 ? path : wall);
        }
        printf("%c\n", wall);

        for(unsigned int column = 0; column < columns; column++)
        {
            // check the wall index left to the current row and column
            printf("%c %c ", column > 0 && readBit(walls, 2 * (row * columns) - row + column - 1) == 0 ? path : wall, path);
        }
        printf("%c\n", wall);
    }

    for(unsigned int column = 0; column < columns; column++)
    {
            printf("%c %c ", wall, wall);
    }
    printf("%c", wall);
}

void saveMaze(unsigned long long wallsLength)
{
    FILE *file = fopen(saveFileName, "w");
    fprintf(file, "%d\n", rows);
    fprintf(file, "%d\n", columns);
        
    for(unsigned long long i = 0; i < wallsLength; i++)
    {
        fprintf(file, "%d\n", walls[i]);
    }

    fclose(file);
}
