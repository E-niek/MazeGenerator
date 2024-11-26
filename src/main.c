#include "main.h"

enum Direction { LEFT, RIGHT, UP, DOWN };

unsigned short rows = 20;
unsigned short columns = 20;
bitset walls;
bitset visited;

int *seed = NULL;
bool printToStdout = true;
bool saveAsMaze = false;
bool saveAsImg = false;
char saveFileName[100] = "maze";
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
        "   -r: " "number of rows\n"
        "   -c: " "number of columns\n"
        "   -s: " "seed\n"
        "   -o: " "output; img, stdout\n"
        "   -n: " "if the output is a img, specify the name of the img here\n"
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
                saveAsMaze = strstr(optarg, "file") != NULL ? true : false;
                saveAsImg = strstr(optarg, "img") != NULL ? true : false;
                break;
            case 'n':
                if(strlen(optarg) > 100)
                {
                    printf("ERROR: Filename exceeds max length of 100\n");
                    exit(1);
                }
                strcpy(saveFileName, optarg);
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

    if(saveAsMaze)
    {
        saveMaze(wallsLength);
    }
    if(saveAsImg)
    {
        saveImg();
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

    for(unsigned short row = 0; row < rows; row++)
    {
        for(unsigned short column = 0; column < columns; column++)
        {
            // check the wall index above the current row and column
            printf("%c %c ", wall, row > 0 && readBit(walls, 2 * (row * columns) - row - columns + column) == 0 ? path : wall);
        }
        printf("%c\n", wall);

        for(unsigned short column = 0; column < columns; column++)
        {
            // check the wall index left to the current row and column
            printf("%c %c ", column > 0 && readBit(walls, 2 * (row * columns) - row + column - 1) == 0 ? path : wall, path);
        }
        printf("%c\n", wall);
    }

    for(unsigned short column = 0; column < columns; column++)
    {
            printf("%c %c ", wall, wall);
    }
    printf("%c", wall);
}

void saveMaze(unsigned long long wallsLength)
{
    char *fileNameBuffer = saveFileName;

    FILE *file = fopen(strcat(fileNameBuffer, ".maze"), "w");
    fprintf(file, "%d\n", rows);
    fprintf(file, "%d\n", columns);

    for(unsigned long long i = 0; i < wallsLength; i++)
        {
        fprintf(file, "%d\n", walls[i]);
    }

    fclose(file);
}

void saveImg()
{
    char *fileNameBuffer = saveFileName;

    unsigned int width = columns * 2 + 1;
    unsigned int height = rows * 2 + 1;

    uint8_t dataBGRA[width * height];

    // this function is not intended for this use, but it works
    clearBitset(dataBGRA, width * height);

    dataBGRA[31] = 0;
    
    for(unsigned short row = 0; row < rows * 2; row++)
    {
        for(unsigned short column = 0; column < columns * 2; column++)
        {
           dataBGRA[row * width + column] = 0;
        }

        dataBGRA[row * width + columns * 2] = 0;

        for(unsigned short column = 0; column < columns * 2; column++)
        {

        }
    }

    for(unsigned short column = 0; column < columns * 2; column++)
    {
        dataBGRA[(rows * 2) * width + column] = 0;
    }
    dataBGRA[width * height - 1] = 0;

    writeImage(strcat(fileNameBuffer, ".tga"), width, height, dataBGRA, 1, 3);
}
