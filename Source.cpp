#include <iostream>
#include <dos.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "Point.h"
#include "Snake.h"
using namespace std;

int main()
{
	srand(time(NULL));

	Snake snake;
	char op = '1';

	do {
		if (_kbhit())								//get key for moving
		{
			op = _getch();
		}
		switch (op) 
		{
			case 'w':
				snake.TurnUp();
				break;
			case 's':
				snake.TurnDown();
				break;
			case 'a':
				snake.TurnLeft();
				break;
			case 'd':
				snake.TurnRight();
				break;
		}
		snake.Move();
	} while (op != 'e');

	return 0;
}