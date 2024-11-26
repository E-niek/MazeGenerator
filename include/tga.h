// This code is stolen from a Github gist :), so, thanks Nick Klingensmith
// Check it out here: https://gist.github.com/maluoi/ade07688e741ab188841223b8ffeed22

#ifndef TGA_H
#define TGA_H

#include <stdio.h>
#include <stdint.h>

void writeImage(const char *filename, uint32_t width, uint32_t height, uint8_t *dataBGRA, uint8_t dataChannels, uint8_t fileChannels)
{
	FILE *fp = NULL;
	fp = fopen(filename, "wb");
	if (fp == NULL) return;

	uint8_t header[18] = { 0,0,2,0,0,0,0,0,0,0,0,0, (uint8_t)(width%256), (uint8_t)(width/256), (uint8_t)(height%256), (uint8_t)(height/256), (uint8_t)(fileChannels*8), 0x20 };
	fwrite(&header, 18, 1, fp);

	for (uint32_t i = 0; i < width*height; i++)
	{
		for (uint32_t b = 0; b < fileChannels; b++)
		{
			fputc(dataBGRA[(i*dataChannels) + (b%dataChannels)], fp);
		}
	}
	fclose(fp);
}

#endif