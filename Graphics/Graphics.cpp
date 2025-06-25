#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "graphics.h"

#define SIZE		5
#define MAX_BOMBS	10
#define TILE_SIZE	100
#define OFFSETX		(1024 - (SIZE * TILE_SIZE)) / 2
#define OFFSETY     (768 - (SIZE * TILE_SIZE)) / 2
#define BOMBTIME	5 * 1000
#define START_TIME	60 * 1000	

#define BOMB	-1
#define MOLE	1
#define TIMER	2
#define EMPTY	0


struct IMAGES
{
	unsigned char* mole;
	unsigned char* hole;
	unsigned char* bomb;
	unsigned char* timer;
};

struct BOMBE
{
	COORD coordinaten;
	unsigned int time;
};


void readImages(IMAGES* images); // Bilder in den Speicher laden
void showImage(unsigned char* image, int row, int col);
void initGamefield(int gamefield[][SIZE], IMAGES* images);
bool HitMole(int gamefield[][SIZE] , int row, int col);
void showScore(int molesHit);
void placemole(int gamefield[][SIZE], IMAGES* images, int row, int col, BOMBE bomben[]);
void placebomb(int gamefield[][SIZE], IMAGES* images, int row, int col, BOMBE bomben[]);
bool HitBomb(int gamefield[][SIZE], int row, int col);
void bombtimer(int gamefield[][SIZE], BOMBE bomben[], IMAGES* images);
void placeTimer(int gamefield[][SIZE], IMAGES* images, int row, int col);


void main()
{
	int window;
	int gamefield[SIZE][SIZE];
	int mouseX;
	int mouseY;
	IMAGES images;
	int molesHit = 0;
	BOMBE bomben[MAX_BOMBS] = { 0 };

	
	window = initwindow(1024, 768, "Whack-A-Mole");
	setcurrentwindow(window);
	srand(time(NULL));

	settextstyle(BOLD_FONT, HORIZ_DIR, 0);

	showScore(molesHit);
	
	readImages(&images);

	showImage(images.bomb, 0, 0);

	initGamefield(gamefield, &images);

	placemole(gamefield, &images, 0, 0, bomben);
	while (true)
	{
		bombtimer(gamefield, bomben, &images);

		if (ismouseclick(WM_LBUTTONDOWN))
		{
			//Linke Maustaste			
			getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);

			if (mouseX - OFFSETX >= 0 && mouseX - OFFSETX <= SIZE * TILE_SIZE && mouseY - OFFSETY >= 0 && mouseY - OFFSETY <= SIZE * TILE_SIZE)
			{
				//Mausposition ist im Spielfeld

				int col = (mouseX - OFFSETX) / TILE_SIZE;
				int row = (mouseY - OFFSETY) / TILE_SIZE;

				if (HitMole(gamefield , row, col))
				{
					molesHit++;
					showImage(images.hole, row, col);
					showScore(molesHit);
					placemole(gamefield, &images, row, col, bomben);
				}
				if (HitBomb(gamefield, row, col))
				{
					molesHit -= 10;
					showImage(images.hole, row, col);
					showScore(molesHit);
				}
			}
		}

		Sleep(10);
	}

	_getch();
}

void placeTimer(int gamefield[][SIZE], IMAGES* images, int row, int col)
{
	int randomX;
	int randomY;

	do {
		randomX = rand() % SIZE;
		randomY = rand() % SIZE;
	} while (randomX == col && randomY == row);

	gamefield[row][col] = TIMER;
	showImage(images->timer, randomY, randomX);
}

void bombtimer(int gamefield[][SIZE], BOMBE bomben[], IMAGES* images)
{
	int row;
	int col;
	unsigned int currenttime = clock();

	for (int i = 0; i < MAX_BOMBS; i++)
	{
		if (bomben[i].time != 0)
		{
			if (currenttime - bomben[i].time >= BOMBTIME)
			{
				bomben[i].time = 0;

				row = bomben[i].coordinaten.Y;
				col = bomben[i].coordinaten.X;

				showImage(images->hole, row, col);
				gamefield[row][col] = EMPTY;
			}
		}
	}
}

bool HitBomb(int gamefield[][SIZE], int row, int col)
{
	if (gamefield[row][col] == BOMB)
	{
		gamefield[row][col] = EMPTY;
		return true;
	}
	if (gamefield[row][col] == EMPTY || gamefield[row][col] == MOLE)
	{
		return false;
	}
}

void placebomb(int gamefield[][SIZE], IMAGES* images, int row, int col, BOMBE bomben[])
{
	int randomX;
	int randomY;

	do {
		randomX = rand() % SIZE;
		randomY = rand() % SIZE;		
	} while (randomX == col && randomY == row);

	for (int i = 0; i < MAX_BOMBS; i++)
	{
		if (bomben[i].time == 0)
		{
			bomben[i].coordinaten.X = randomX;
			bomben[i].coordinaten.Y = randomY;
			bomben[i].time = clock();

			gamefield[randomY][randomX] = BOMB;
			showImage(images->bomb, randomY, randomX);
			break;
		}
	}

}

void showScore(int molesHit)
{
	char buffer[30];

	setfillstyle(SOLID_FILL, BLACK);
	bar(350, 20, 700, 80);

	sprintf(buffer, "Score: %3d", molesHit);

	outtextxy(435, 50, buffer);
}

void placemole(int gamefield[][SIZE], IMAGES* images, int row, int col, BOMBE bomben[])
{
	int randomX;
	int randomY;
	int randombomb;
	int randomtimer;
	bool flag = true;

	do{
		randomX = rand() % SIZE;
		randomY = rand() % SIZE;
		randombomb = rand() % 6;
		randomtimer = rand() % 10;

		if (randomX != col && randomY != row)
		{
			for (int i = 0; i < MAX_BOMBS; i++)
			{
				flag = false;

				if (bomben[i].time != 0)
				{
					if (bomben[i].coordinaten.X == randomX && bomben[i].coordinaten.Y == randomY)
					{
						flag = true;
						break;
					}
				}
			}
		}

	} while (flag);
	
	if (randombomb == 3)
	{
		placebomb(gamefield, images, randomY, randomX, bomben);
	}
	if (randomtimer == 5)
	{
		placeTimer(gamefield, images, row, col);
	}

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
	if (gamefield[row][col] == EMPTY || gamefield[row][col] == BOMB)
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

	readimagefile(".\\Images\\Bomb.bmp", 100, 100, 200, 200);
	images->bomb = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->bomb);

	readimagefile(".\\Images\\Timer.bmp", 100, 100, 200, 200);
	images->timer = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->timer);

	bar(100, 100, 201, 201);
}
