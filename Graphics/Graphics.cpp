#include <stdio.h>
#include <conio.h>
#include "graphics.h"

#define SIZE		3
#define TILE_SIZE	70

struct IMAGES
{
	unsigned char* mole;
	unsigned char* hole;
	unsigned char* hammer;
};

void readImages(IMAGES* images); // Bilder in den Speicher laden
void showImage(unsigned char* image, int row, int col);
void initGamefield(int gamefield[][SIZE], IMAGES* images);


void main()
{
	int window;
	int gamefield[SIZE][SIZE];
	//int mouseX;
	//int mouseY;
	IMAGES images;
	
	window = initwindow(1024, 768, "Whack-A-Mole");
	setcurrentwindow(window);

	readImages(&images);

	initGamefield(gamefield, &images);



	_getch();
}

void initGamefield(int gamefield[][SIZE], IMAGES* images)
{
	for (int row = 0; row < SIZE; row++)
	{
		for (int col = 0; col < SIZE; col++)
		{
			showImage(images->hole, row, col);
		}
	}
}


void showImage(unsigned char* image, int row, int col)
{
	putimage(col * TILE_SIZE, row * TILE_SIZE, image, COPY_PUT);
}

void readImages(IMAGES* images)
{
	readimagefile(".\\Images\\Hole.png", 100, 100, 171, 171);
	images->hole = (unsigned char*)malloc(imagesize(100, 100, 171, 171));
	getimage(100, 100, 171, 171, images->hole);

	readimagefile(".\\Images\\Mole.png", 100, 100, 171, 171);
	images->mole = (unsigned char*)malloc(imagesize(100, 100, 171, 171));
	getimage(100, 100, 171, 171, images->mole);

	readimagefile(".\\Images\\Hammer.png", 100, 100, 171, 171);
	images->hammer = (unsigned char*)malloc(imagesize(100, 100, 171, 171));
	getimage(100, 100, 171, 171, images->hammer);
}
