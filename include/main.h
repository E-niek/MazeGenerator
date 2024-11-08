#ifndef MAIN_H
#define MAIN_H

#include "bitset.h"
#include "random.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

void allocateMemory(bool allocateVisited);
void generateMaze(int row, int column);
int possibleDirection(int row, int column, int side);
void printStdout();
void loadMaze();
void saveMaze();

#endif