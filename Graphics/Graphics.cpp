#include <stdio.h>
#include <conio.h>
#include "graphics.h"

struct IMAGES
{
	unsigned char* mole;
	unsigned char* hole;
	unsigned char* hammer;
};

void readImages(IMAGES* images); // Bilder in den Speicher laden


void main()
{
	int window;
	
	window = initwindow(1024, 768, "Grafiktest");
	
	setcurrentwindow(window);

	line(0, 0, getmaxx(), getmaxy());

	_getch();
}

void readImages(IMAGES* images)
{
	readimagefile(".\\Images\\", 100, 100, 171, 171);
	images->unopened = (unsigned char*)malloc(imagesize(100, 100, 171, 171));
	getimage(100, 100, 171, 171, images->unopened);

}
