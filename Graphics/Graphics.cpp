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
};

void readImages(IMAGES* images); // Bilder in den Speicher laden
void showImage(unsigned char* image, int row, int col);
void initGamefield(int gamefield[][SIZE], IMAGES* images);
bool HitMole(int gamefield[][SIZE] , int row, int col);
void showHitMoles(int molesHit);
void placemole(int gamefield[][SIZE], IMAGES* images);


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
	srand(time(NULL));

	settextstyle(BOLD_FONT, HORIZ_DIR, 0);

	showHitMoles(molesHit);
	
	readImages(&images);
	initGamefield(gamefield, &images);

	placemole(gamefield, &images);
	while (true)
	{
		if (ismouseclick(WM_LBUTTONDOWN))
		{
			//Linke Maustaste			
			getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);

			if (mouseX >= 0 && mouseX <= SIZE * TILE_SIZE && mouseY >= 0 && mouseY <= SIZE * TILE_SIZE)
			{
				//Mausposition ist im Spielfeld

				int col = (mouseX - OFFSETX) / TILE_SIZE;
				int row = (mouseY - OFFSETY) / TILE_SIZE;

				if (HitMole(gamefield , row, col))
				{
					molesHit++;
					showImage(images.hole, row, col);
					showHitMoles(molesHit);
					placemole(gamefield, &images);
				}
			}
		}

		Sleep(10);
	}

	_getch();
}

void showHitMoles(int molesHit)
{
	char buffer[15];

	setfillstyle(SOLID_FILL, BLACK);
	bar(700, 100, 800, 730);

	sprintf(buffer, "Score: %d", molesHit);

	outtextxy(700, 100, buffer);
}


void placemole(int gamefield[][SIZE], IMAGES* images)
{
	int randomX = rand() % SIZE;
	int randomY = rand() % SIZE;

	gamefield[randomY][randomX] = MOLE;
	showImage(images->mole, randomY, randomX);
}



bool HitMole(int gamefield[][SIZE], int row, int col)
{
	if (gamefield[row][col] == MOLE)
	{
		gamefield[row][col] = EMPTY;
		return true;
	}
	if (gamefield[row][col] == EMPTY)
	{
		return false;
	}
	
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
