#include <stdio.h>
#include <conio.h>
#include "graphics.h"

#define SIZE		5
#define TILE_SIZE	100
#define OFFSETX		40
#define OFFSETY     20

#define MOLE	1
#define EMPTY	0

struct IMAGES
{
	unsigned char* mole;
	unsigned char* hole;
	unsigned char* hammer;
};

void readImages(IMAGES* images); // Bilder in den Speicher laden
void showImage(unsigned char* image, int row, int col);
void initGamefield(int gamefield[][SIZE], IMAGES* images);
void placemole(int gamefield[][SIZE], IMAGES* images);

void main()
{
	int window;
	int gamefield[SIZE][SIZE];
	//int mouseX;
	//int mouseY;
	IMAGES images;
	
	window = initwindow(1024, 768, "Whack-A-Mole");
	setcurrentwindow(window);
	srand(time(NULL));


	readImages(&images);
	initGamefield(gamefield, &images);

	placemole(gamefield, &images);


	_getch();
}


void placemole(int gamefield[][SIZE], IMAGES* images)
{
	int randomX = rand() % SIZE;
	int randomY = rand() % SIZE;

	gamefield[randomY][randomX] = MOLE;
	showImage(images->mole, randomY, randomX);
}


void initGamefield(int gamefield[][SIZE], IMAGES* images)
{
	for (int row = 0; row < SIZE; row++)
	{
		for (int col = 0; col < SIZE; col++)
		{
			gamefield[row][col] = EMPTY;
			showImage(images->hole, row, col);
		}
	}
}


void showImage(unsigned char* image, int row, int col)
{
	putimage(col * TILE_SIZE + OFFSETX, row * TILE_SIZE + OFFSETY, image, COPY_PUT);
}

void readImages(IMAGES* images)
{
	readimagefile(".\\Images\\Hole.bmp", 100, 100, 200, 200);
	images->hole = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->hole);

	readimagefile(".\\Images\\Mole.bmp", 100, 100, 200, 200);
	images->mole = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->mole);
}
