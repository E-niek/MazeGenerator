#ifndef MAIN_H
#define MAIN_H

#include "bitset.h"
#include "random.h"
#include "tga.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

unsigned long long allocateMemory(bool allocateVisited);
void generateMaze(unsigned int row, unsigned int column);
int possibleDirection(unsigned int row, unsigned int column, uint8_t side);
void printStdout();
void loadMaze();
void saveMaze(unsigned long long wallsLength);
void saveImg();

#endif