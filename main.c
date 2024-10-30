#include "headers/main.h"

unsigned int rows = 20;
unsigned int columns = 20;
char wall = '#';
char path = ' ';

int main(int argc, char **argv)
{
    for(int i = 1; i < argc; i++)
    {
        rows = !strcmp(argv[i], "-r") ? atoi(argv[i + 1]) : rows;
        columns = !strcmp(argv[i], "-c") ? atoi(argv[i + 1]) : columns;
    }

    bitset maze = {4, 5};

    // writeBit(maze, 1, 7, 1);
    printf("Ouput: %d\n", readBit(maze, 0, 3));
    // printf("Ouput: %d\n", 1 << 2);
    // printf("Ouput: %d\n",  4 & 4);


    for(int i = 0; i < 2; i++)
    {
        printf("Maze value %d: %d\n", i, maze[i]);
    }

    // printMaze();
}

void printMaze()
{
    for(int row = 0; row < rows * 2 + 1; row++)
    {
        for(int column = 0; column < columns; column++)
        {
            // if row is odd print a path, else a wall
            printf("%c %c ", wall, row % 2 == 1 ? path : wall);
        }
        printf("%c\n", wall);
    }
}