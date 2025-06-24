#include <stdio.h>
#include <conio.h>
#include "graphics.h"

//#define SIZE		3
//#define TILE_SIZE	

struct IMAGES
{
	unsigned char* mole;
	unsigned char* hole;
	unsigned char* hammer;
};

void readImages(IMAGES* images); // Bilder in den Speicher laden
//void initGamefield(int gamefield[][SIZE]);


void main()
{
	int window;
	//int gamefield[SIZE][SIZE];
	//int mouseX;
	//int mouseY;
	IMAGES images;
	
	window = initwindow(1024, 768, "Whack-A-Mole");
	
	setcurrentwindow(window);

	readImages(&images);

	_getch();
}

//void initGamefield(int gamefield[][SIZE])
//{
//
//}

void readImages(IMAGES* images)
{
	readimagefile(".\\Images\\Hole.bmp", 100, 100, 171, 171);
	images->hole = (unsigned char*)malloc(imagesize(100, 100, 171, 171));
	getimage(100, 100, 171, 171, images->hole);

	readimagefile(".\\Images\\Mole.bmp", 100, 100, 171, 171);
	images->mole = (unsigned char*)malloc(imagesize(100, 100, 171, 171));
	getimage(100, 100, 171, 171, images->mole);

	readimagefile(".\\Images\\Hammer.bmp", 100, 100, 171, 171);
	images->hammer = (unsigned char*)malloc(imagesize(100, 100, 171, 171));
	getimage(100, 100, 171, 171, images->hammer);
}
