#pragma once]

class Snake
{
private:
	Point* cell[MaxSnakeSize];
	int size;
	char dir;
	Point fruit;
	int state;

public:
	int started;
	Snake()
	{
		size = 1;
		cell[0] = new Point(20, 20);

		for (int i = 1; i < MaxSnakeSize; i++)
		{
			cell[i] = NULL;
		}
		fruit.SetPoint(rand() % MaxFrameX, rand() % MaxFrameY);
		state = 0;

	}
	void AddCell(int x, int y)
	{
		cell[size++] = new Point(x, y);
	}
	void TurnUp()
	{
		if (dir != 's')
			dir = 'w';
	}
	void TurnDown()
	{
		if (dir != 'w')
			dir = 's';
	}
	void TurnLeft()
	{
		if (dir != 'd')
			dir = 'a';
	}
	void TurnRight()
	{
		if (dir != 'a')
			dir = 'd';
	}
	void WelcomeScreen()												//home screen
	{
		SetConsoleTextAttribute(console, 112);
		cout << "Snake 2.0.1 by Marchenko Yur4ik" << endl;
	}

	void Move()
	{
		system("cls");

		if (state == 0)
		{
			if (started)
			{
				WelcomeScreen();
				cout << "Press any key to start";
				_getch();
				started = 1;
				state = 1;
			}
			else
			{
				cout << "Game over";
				cout << "Press any key to start";
				_getch();
				state = 1;
				size = 1;
			}
		}

		for (int i = size - 1; i > 0; i--)
		{
			cell[i - 1]->CopyPos(cell[i]);
		}
		switch (dir)									//control buttons
		{
		case 'w':
			cell[0]->MoveUp();
			break;
		case 's':
			cell[0]->MoveDown();
			break;
		case 'a':
			cell[0]->MoveLeft();
			break;
		case 'd':
			cell[0]->MoveRight();
			break;
		}

		if (SelfCollision())
		{
			state = 0;
		}

		if (fruit.GetX() == cell[0]->GetX() && fruit.GetY() == cell[0]->GetY())			//colision with fruit
		{
			AddCell(0, 0);
			fruit.SetPoint(rand() % MaxFrameX, rand() % MaxFrameY);
		}

		for (int i = 0; i < size; i++)				//drawing snake
			cell[i]->Draw();
		SetConsoleTextAttribute(console, 192);
		fruit.Draw('.');
		SetConsoleTextAttribute(console, 112);
		Sleep(10);
	}

	int SelfCollision()									//collision in itself
	{
		for (int i = 1; i < size; i++)
			if (cell[0]->IsEqual(cell[i]))
				return 1;
		return 0;
	}
};
