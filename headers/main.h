#ifndef MAIN_H
#define MAIN_H

#include "utilities.h"
#include "bitset.h"
#include "random.h"

void generateMaze(bitset maze, int row, int column, int lastRow, int lastColumn);
int possibleDirection(bitset maze, int row, int column, int side);
void printMaze();

#endif