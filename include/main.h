#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdint.h>

unsigned long long allocateMemory(bool allocateVisited);
void generateMaze(unsigned int row, unsigned int column);
int possibleDirection(unsigned int row, unsigned int column, uint8_t side);
void printStdout();
void loadMaze(char *loadMazeName);
void saveMaze(unsigned int wallsLength, char *saveMazeName);
void saveImg(char *saveImgName);

#endif
