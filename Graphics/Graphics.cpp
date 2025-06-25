#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "graphics.h"

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768

#define SIZE		6
#define MAX_BOMBS	10
#define TILE_SIZE	100
#define OFFSETX		(WINDOW_WIDTH - (SIZE * TILE_SIZE)) / 2
#define OFFSETY     (WINDOW_HEIGHT - (SIZE * TILE_SIZE)) / 2
#define MOLE_TIME	2 * 1000
#define BOMBTIME	5 * 1000
#define START_TIME	60 * 1000	

#define CLOCK		2
#define MOLE		1
#define EMPTY		0
#define CLOCKBOMB	-2
#define BOMB		-1


struct IMAGES
{
	unsigned char* mole;
	unsigned char* hole;
	unsigned char* bomb;
	unsigned char* clock;
	unsigned char* clockbomb;
};

struct OBJEKT
{
	COORD coordinaten;
	unsigned int time;
};


void readImages(IMAGES* images); // Bilder in den Speicher laden
void initGamefield(int gamefield[][SIZE], IMAGES* images);
void showImage(unsigned char* image, int row, int col);
void showScore(int molesHit);
void placemole(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT bomben[], OBJEKT* mole);
void placebomb(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT bomben[]);
void placeClock(int gamefield[][SIZE], IMAGES* images, int row, int col);
bool HitMole(int gamefield[][SIZE] , int row, int col);
bool HitBomb(int gamefield[][SIZE], int row, int col);
bool HitClock(int gamefield[][SIZE], int row, int col);
void ShowTime(unsigned int gametime);
void bombtimer(int gamefield[][SIZE], OBJEKT bomben[], IMAGES* images);
void moletimer(int gamefield[][SIZE], OBJEKT* mole, IMAGES* images, OBJEKT bomben[]);

void main()
{
	int window;
	int gamefield[SIZE][SIZE];
	int mouseX;
	int mouseY;
	IMAGES images;
	int molesHit = 0;
	OBJEKT bomben[MAX_BOMBS] = { 0 };
	OBJEKT mole;
	unsigned int startingtime;
	

	
	window = initwindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Whack-A-Mole");
	setcurrentwindow(window);
	srand(time(NULL));
	startingtime = clock() * 1000 / CLOCKS_PER_SEC;

	readImages(&images);
	initGamefield(gamefield, &images);

	settextstyle(BOLD_FONT, HORIZ_DIR, 0);
	showScore(molesHit);

	placemole(gamefield, &images, 0, 0, bomben, &mole);
	while (true)
	{
		bombtimer(gamefield, bomben, &images);
		moletimer(gamefield, &mole, &images, bomben);
		ShowTime(startingtime);

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
					placemole(gamefield, &images, row, col, bomben, &mole);
				}

				if (HitClock(gamefield, row, col))
				{
					
					showImage(images.hole, row, col);
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

	setfillstyle(SOLID_FILL, BLACK);
	bar(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	outtextxy(OFFSETX, OFFSETY, (char*)"Game Over!");

	_getch();
}

bool HitClock(int gamefield[][SIZE], int row, int col)
{
	if (gamefield[row][col] == CLOCK)
	{
		gamefield[row][col] = EMPTY;
		return true;
	}

	return false;
}

void ShowTime(unsigned int gametime)
{
	unsigned int currentTime = clock() * 1000 / CLOCKS_PER_SEC;
	int displayTime;

	displayTime = currentTime - gametime;

	if (displayTime < 0)
	{
		return;
	}

	char buffer[30];

	setfillstyle(SOLID_FILL, BLACK);
	bar(600, 20, 900, 80);

	sprintf(buffer, "Time: %d", (START_TIME - displayTime) / 1000);

	outtextxy(650, 30, buffer);
}

void moletimer(int gamefield[][SIZE], OBJEKT* mole, IMAGES* images, OBJEKT bomben[])
{
	unsigned int currenttime = clock() * 1000 / CLOCKS_PER_SEC;
	int row;
	int col;

	if (currenttime - mole->time > MOLE_TIME)
	{
		row = mole->coordinaten.Y;
		col = mole->coordinaten.X;

		showImage(images->hole, row, col);
		gamefield[row][col] = EMPTY;

		placemole(gamefield, images, row, col, bomben, mole);
	}
}

void placeClock(int gamefield[][SIZE], IMAGES* images, int row, int col)
{
	int randomX;
	int randomY;

	do {
		randomX = rand() % SIZE;
		randomY = rand() % SIZE;

		if (randomX != col && randomY != row)
		{
			if (gamefield[randomY][randomX] == EMPTY)
			{
				break;
			}
		}

	} while (true);

	gamefield[randomY][randomX] = CLOCK;
	showImage(images->clock, randomY, randomX);
}

void bombtimer(int gamefield[][SIZE], OBJEKT bomben[], IMAGES* images)
{
	int row;
	int col;
	unsigned int currenttime = clock() * 1000 / CLOCKS_PER_SEC;

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
	
	return false;
}

void placebomb(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT bomben[])
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
			bomben[i].time = clock() * 1000 / CLOCKS_PER_SEC;

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
	bar(OFFSETX, 30, 600, 80);

	sprintf(buffer, "Score: %4d", molesHit);

	outtextxy(OFFSETX, 30, buffer);
}

void placemole(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT bomben[], OBJEKT* mole)
{
	int randomX;
	int randomY;
	int randombomb;
	int randomclock;
	bool flag = true;

	do{
		randomX = rand() % SIZE;
		randomY = rand() % SIZE;
		randombomb = rand() % 6;
		randomclock = rand() % 10;

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
	if (randomclock == 5)
	{
		placeClock(gamefield, images, randomY, randomX);
	}

	mole->coordinaten.X = randomX;
	mole->coordinaten.Y = randomY;
	mole->time = clock() * 1000 / CLOCKS_PER_SEC;

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
	
	return false;
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
	images->clock = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->clock);

	readimagefile(".\\Images\\TimerBomb.bmp", 100, 100, 200, 200);
	images->clockbomb = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->clockbomb);

	setfillstyle(SOLID_FILL, BLACK);
	bar(100, 100, 201, 201);
}
