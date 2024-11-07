#ifndef MAIN_H
#define MAIN_H

#include "utilities.h"
#include "bitset.h"
#include "random.h"

void generateMaze(int row, int column);
int possibleDirection(int row, int column, int side);
void printMaze();

#endif