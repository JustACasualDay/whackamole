#include <stdio.h>
#include <conio.h>
#include "graphics.h"

#define SIZE		5
#define TILE_SIZE	100

struct IMAGES
{
	unsigned char* mole;
	unsigned char* hole;
};

void readImages(IMAGES* images); // Bilder in den Speicher laden
void showImage(unsigned char* image, int row, int col);
void initGamefield(int gamefield[][SIZE], IMAGES* images);
bool HitMole(int gamefield[][SIZE], IMAGES* images, int row, int col, int& molesHit);



void main()
{
	int window;
	int gamefield[SIZE][SIZE];
	int mouseX;
	int mouseY;
	IMAGES images;
	int molesHit = 0;
	
	window = initwindow(1024, 768, "Whack-A-Mole");
	setcurrentwindow(window);

	readImages(&images);

	initGamefield(gamefield, &images);

	while (true)
	{
		if (ismouseclick(WM_LBUTTONDOWN))
		{
			//Linke Maustaste			
			getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);

			if (mouseX >= 0 && mouseX <= SIZE * TILE_SIZE && mouseY >= 0 && mouseY <= SIZE * TILE_SIZE)
			{
				//Mausposition ist im Spielfeld

				int col = mouseX / TILE_SIZE;
				int row = mouseY / TILE_SIZE;

				if (HitMole(gamefield, &images, row, col, molesHit))
				{
					molesHit++;
					showImage(images.hole, row, col);
				}
			}
		}

		Sleep(10);
	}

	_getch();
}


bool HitMole(int gamefield[][SIZE], IMAGES* images, int row, int col, int& molesHit)
{

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
	readimagefile(".\\Images\\Hole.bmp", 100, 100, 200, 200);
	images->hole = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->hole);

	readimagefile(".\\Images\\Mole.bmp", 100, 100, 200, 200);
	images->mole = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->mole);
}
