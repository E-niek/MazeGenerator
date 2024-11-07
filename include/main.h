#ifndef MAIN_H
#define MAIN_H

#include "bitset.h"
#include "random.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void generateMaze(int row, int column);
int possibleDirection(int row, int column, int side);
void printMaze();

#endif