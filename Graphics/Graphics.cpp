#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "graphics.h"

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768

#define SIZE			6
#define MAX_BOMBS		10
#define MAX_CLOCKBOMBS	10
#define MAX_CLOCKS		10
#define MAX_MOLES		3
#define TILE_SIZE		100
#define OFFSETX			(WINDOW_WIDTH - (SIZE * TILE_SIZE)) / 2
#define OFFSETY			(WINDOW_HEIGHT - (SIZE * TILE_SIZE)) / 2
#define START_TIME		60
#define MOLE_TIME		1
#define CLOCK_TIME		1
#define BOMB_TIME		3

#define CLOCK			2
#define MOLE			1
#define EMPTY			0
#define CLOCKBOMB		-2
#define BOMB			-1

#define COOLDOWN		1000 // DO NOT CHANGE


struct IMAGES
{
	unsigned char* mole;
	unsigned char* hole;
	unsigned char* bomb;
	unsigned char* clock;
	unsigned char* clockbomb;
	unsigned char* restartbtn;
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
bool checkAvailable(int row, int col, OBJEKT moles[], OBJEKT clocks[], OBJEKT bombs[], OBJEKT clockbombs[]);
void placeBomb(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT moles[], OBJEKT clocks[], OBJEKT bombs[], OBJEKT clockbombs[]);
void placeClock(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT moles[], OBJEKT clocks[], OBJEKT bombs[], OBJEKT clockbombs[]);
void placeMole(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT moles[], OBJEKT clocks[], OBJEKT bombs[], OBJEKT clockbombs[]);
void placeClockBomb(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT moles[], OBJEKT clocks[], OBJEKT bombs[], OBJEKT clockbombs[]);
bool HitMole(int gamefield[][SIZE] , int row, int col);
bool HitBomb(int gamefield[][SIZE], int row, int col);
bool HitClock(int gamefield[][SIZE], int row, int col);
void ShowTime(unsigned int startingtime, int& gametime);
void updateTimers(int gamefield[][SIZE], IMAGES* images, OBJEKT bombs[], OBJEKT clocks[], OBJEKT moles[], OBJEKT clockbombs[], unsigned int& cooldowntimer);
bool restart(int molesHit, unsigned char* image);

void main()
{
	int window;
	window = initwindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Whack-A-Mole");
	setcurrentwindow(window);

	start:
	
	int gamefield[SIZE][SIZE];
	int mouseX;
	int mouseY;
	IMAGES images;
	int molesHit = 0;
	OBJEKT bombs[MAX_BOMBS] = { 0 };
	OBJEKT clocks[MAX_CLOCKS] = { 0 };
	OBJEKT moles[MAX_MOLES] = { 0 };
	OBJEKT clockbombs[MAX_CLOCKBOMBS] = { 0 };
	unsigned int startingtime;
	int gametime;
	unsigned int cooldowntimer;

	srand(time(NULL));
	startingtime = clock() * 1000 / CLOCKS_PER_SEC;
	cooldowntimer = clock() * 1000 / CLOCKS_PER_SEC;
	gametime = START_TIME * 1000;

	readImages(&images);
	initGamefield(gamefield, &images);

	settextstyle(BOLD_FONT, HORIZ_DIR, 0);
	showScore(molesHit);

	for (int i = 0; i < MAX_MOLES; i++)
	{
		placeMole(gamefield, &images, -1, -1, moles, clocks, bombs, clockbombs);
	}

	while (gametime > 0)
	{
		ShowTime(startingtime, gametime);
		updateTimers(gamefield, &images, bombs, clocks, moles, clockbombs, cooldowntimer);

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
					placeMole(gamefield, &images, -1, -1, moles, clocks, bombs, clockbombs);
				}

				if (HitClock(gamefield, row, col))
				{
					gametime += 10 * 1000;
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

	if (restart(molesHit, images.restartbtn))
	{
		goto start;
	}

	_getch();
}

bool restart(int molesHit, unsigned char* image)
{
	setfillstyle(SOLID_FILL, BLACK);
	bar(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	outtextxy(OFFSETX * 2, OFFSETY * 2, (char*)"Game Over!");

	char buffer[30];

	setfillstyle(SOLID_FILL, BLACK);
	bar(OFFSETX, 400, 600, 420);

	sprintf(buffer, "Score: %4d", molesHit);

	outtextxy(OFFSETX, 400, buffer);

	putimage( 650, 365, image, COPY_PUT);

	int mouseX;
	int mouseY;

	while (true)
	{
		if (ismouseclick(WM_LBUTTONDOWN))
		{
			//Linke Maustaste			
			getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);

			if (mouseX >= 650 && mouseX <= 850 && mouseY >= 365 && mouseY <= 565)
			{
				return true;
			}
		}
	}

}

void updateTimers(int gamefield[][SIZE], IMAGES* images, OBJEKT bombs[], OBJEKT clocks[], OBJEKT moles[], OBJEKT clockbombs[], unsigned int& cooldowntimer)
{
	unsigned int currenttime = clock() * 1000 / CLOCKS_PER_SEC;
	int row;
	int col;
	int genbomb;
	int genclock;
	int genclockbomb;


	// Moles
	for (int i = 0; i < MAX_MOLES; i++)
	{
		if (moles[i].time != 0)
		{
			if (currenttime - moles[i].time > MOLE_TIME * 1000)
			{
				moles[i].time = 0;

				row = moles[i].coordinaten.Y;
				col = moles[i].coordinaten.X;

				showImage(images->hole, row, col);
				gamefield[row][col] = EMPTY;

				placeMole(gamefield, images, -1, -1, moles, clocks, bombs, clockbombs);
			}
		}
	}

	// Bombs
	for (int i = 0; i < MAX_BOMBS; i++)
	{
		if (bombs[i].time != 0)
		{
			if (currenttime - bombs[i].time >= BOMB_TIME * 1000)
			{
				bombs[i].time = 0;

				row = bombs[i].coordinaten.Y;
				col = bombs[i].coordinaten.X;

				showImage(images->hole, row, col);
				gamefield[row][col] = EMPTY;
			}
		}
	}

	// Clocks
	for (int i = 0; i < MAX_CLOCKS; i++)
	{
		if (clocks[i].time != 0)
		{
			if (currenttime - clocks[i].time >= CLOCK_TIME * 1000)
			{
				clocks[i].time = 0;

				row = clocks[i].coordinaten.Y;
				col = clocks[i].coordinaten.X;

				showImage(images->hole, row, col);
				gamefield[row][col] = EMPTY;
			}
		}
	}

	if (currenttime - cooldowntimer > COOLDOWN)
	{
		genbomb = rand() % 2;
		genclock = rand() % 20;
		genclockbomb = rand() % 40;

		if (genbomb == 1)
		{
			placeBomb(gamefield, images, -1, -1, moles, clocks, bombs, clockbombs);
		}

		if (genclock == 4)
		{
			placeClock(gamefield, images, -1, -1, moles, clocks, bombs, clockbombs);
		}

		if (genclockbomb == 1)
		{
			placeClockBomb(gamefield, images, -1, -1, moles, clocks, bombs, clockbombs);
		}

		cooldowntimer = clock() * 1000 / CLOCKS_PER_SEC;
	}
}

void ShowTime(unsigned int startingtime, int& gametime)
{
	static unsigned int lastUpdate = 0;
	unsigned int currentTime = clock() * 1000 / CLOCKS_PER_SEC;

	if (lastUpdate == 0)
	{
		lastUpdate = currentTime;
	}

	int delta = currentTime - lastUpdate;
	lastUpdate = currentTime;

	gametime -= delta;


	if (gametime < 0)
	{
		gametime = 0;
	}


	char buffer[30];

	setfillstyle(SOLID_FILL, BLACK);
	bar(600, 20, 900, 80);

	sprintf(buffer, "Time: %d", gametime / 1000);

	outtextxy(650, 30, buffer);
}

void showScore(int molesHit)
{
	char buffer[30];

	setfillstyle(SOLID_FILL, BLACK);
	bar(OFFSETX, 30, 600, 80);

	sprintf(buffer, "Score: %4d", molesHit);

	outtextxy(OFFSETX, 30, buffer);
}

bool checkAvailable(int row, int col, OBJEKT moles[], OBJEKT clocks[], OBJEKT bombs[], OBJEKT clockbombs[])
{
	int i;

	// Moles
	for (i = 0; i < MAX_MOLES; i++)
	{
		if (moles[i].time != 0)
		{
			if (moles[i].coordinaten.X == col && moles[i].coordinaten.Y == row)
			{
				return false;
			}
		}
	}

	// Bombs
	for (i = 0; i < MAX_BOMBS; i++)
	{
		if (bombs[i].time != 0)
		{
			if (bombs[i].coordinaten.X == col && bombs[i].coordinaten.Y == row)
			{
				return false;
			}
		}
	}

	// Clocks
	for (i = 0; i < MAX_CLOCKS; i++)
	{
		if (clocks[i].time != 0)
		{
			if (clocks[i].coordinaten.X == col && clocks[i].coordinaten.Y == row)
			{
				return false;
			}
		}
	}

	// Bombclocks
	for (i = 0; i < MAX_CLOCKBOMBS; i++)
	{
		if (clockbombs[i].time != 0)
		{
			if (clockbombs[i].coordinaten.X == col && clockbombs[i].coordinaten.Y == row)
			{
				return false;
			}
		}
	}

	return true;
}

void placeClockBomb(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT moles[], OBJEKT clocks[], OBJEKT bombs[], OBJEKT clockbombs[])
{
	int randomX;
	int randomY;

	do {
		randomX = rand() % SIZE;
		randomY = rand() % SIZE;
	} while (!checkAvailable(randomY, randomX, moles, clocks, bombs, clockbombs));

	for (int i = 0; i < MAX_CLOCKBOMBS; i++)
	{
		if (clockbombs[i].time == 0)
		{
			clockbombs[i].coordinaten.X = randomX;
			clockbombs[i].coordinaten.Y = randomY;
			clockbombs[i].time = clock() * 1000 / CLOCKS_PER_SEC;

			gamefield[randomY][randomX] = CLOCKBOMB;
			showImage(images->clockbomb, randomY, randomX);
			break;
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

void placeBomb(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT moles[], OBJEKT clocks[], OBJEKT bombs[], OBJEKT clockbombs[])
{
	int randomX;
	int randomY;

	do {
		randomX = rand() % SIZE;
		randomY = rand() % SIZE;		
	} while (!checkAvailable(randomY, randomX, moles, clocks, bombs, clockbombs));

	for (int i = 0; i < MAX_BOMBS; i++)
	{
		if (bombs[i].time == 0)
		{
			bombs[i].coordinaten.X = randomX;
			bombs[i].coordinaten.Y = randomY;
			bombs[i].time = clock() * 1000 / CLOCKS_PER_SEC;

			gamefield[randomY][randomX] = BOMB;
			showImage(images->bomb, randomY, randomX);
			break;
		}
	}

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

void placeClock(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT moles[], OBJEKT clocks[], OBJEKT bombs[], OBJEKT clockbombs[])
{
	int randomX;
	int randomY;

	do {
		randomX = rand() % SIZE;
		randomY = rand() % SIZE;


	} while (!checkAvailable(randomY, randomX, moles, clocks, bombs, clockbombs));

	
	for (int i = 0; i < MAX_CLOCKS; i++)
	{
		if (clocks[i].time == 0)
		{
			clocks[i].coordinaten.X = randomX;
			clocks[i].coordinaten.Y = randomY;
			clocks[i].time = clock() * 1000 / CLOCKS_PER_SEC;

			gamefield[randomY][randomX] = CLOCK;
			showImage(images->clock, randomY, randomX);
			break;
		}
	}
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

void placeMole(int gamefield[][SIZE], IMAGES* images, int row, int col, OBJEKT moles[], OBJEKT clocks[], OBJEKT bombs[], OBJEKT clockbombs[])
{
	int i;
	int randomX;
	int randomY;
	bool flagM = true;

	for (i = 0; i < MAX_MOLES; i++)
	{
		flagM = false;

		if (moles[i].time == 0)
		{
			flagM = true;
			break;
		}
	}

	if (flagM == false)
	{
		return;
	}


	do{
		randomX = rand() % SIZE;
		randomY = rand() % SIZE;

	} while (!checkAvailable(randomY, randomX, moles, clocks, bombs, clockbombs));
	
		
	moles[i].coordinaten.X = randomX;
	moles[i].coordinaten.Y = randomY;
	moles[i].time = clock() * 1000 / CLOCKS_PER_SEC;

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

	readimagefile(".\\Images\\Bomb.bmp", 100, 100, 200, 200);
	images->bomb = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->bomb);

	readimagefile(".\\Images\\Timer.bmp", 100, 100, 200, 200);
	images->clock = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->clock);

	readimagefile(".\\Images\\TimerBomb.bmp", 100, 100, 200, 200);
	images->clockbomb = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->clockbomb);

	readimagefile(".\\Images\\Restart.bmp", 100, 100, 200, 200);
	images->restartbtn = (unsigned char*)malloc(imagesize(100, 100, 200, 200));
	getimage(100, 100, 200, 200, images->restartbtn);

	setfillstyle(SOLID_FILL, BLACK);
	bar(100, 100, 201, 201);
}
