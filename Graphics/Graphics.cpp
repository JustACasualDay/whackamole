#include <stdio.h>
#include <conio.h>
#include "graphics.h"

void main()
{
	int window;
	
	window = initwindow(1024, 768, "Grafiktest");
	
	setcurrentwindow(window);

	line(0, 0, getmaxx(), getmaxy());

	_getch();
}


