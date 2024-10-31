#include "headers/main.h"

// default values
int rows = 20;
int columns = 20;
int seed = 0;

int main(int argc, char **argv)
{
    for(int i = 1; i < argc; i++)
    {
        rows = strcmp(argv[i], "-r") == 0 ? atoi(argv[i + 1]) : rows;
        columns = strcmp(argv[i], "-c") == 0 ? atoi(argv[i + 1]) : columns;
        seed = strcmp(argv[i], "-s") == 0 ? atoi(argv[i] + 1) : seed;
    }

    bitset maze = malloc(ceil((double)(rows * (columns - 1) + (rows - 1) * columns) / 8) + 3 * sizeof(int));

    free(maze);
}

void printMaze()
{
    char wall = '#';
    char path = ' ';

    for(int row = 0; row < rows * 2 + 1; row++)
    {
        for(int column = 0; column < columns; column++)
        {
            // if row is odd print a path, else a wall
            printf("%c %c ", wall, row % 2 == 1 ? path : wall);
        }
        // remove the enter from the last row
        printf(row < rows * 2 ? "%c\n" : "%c", wall);
    }
}