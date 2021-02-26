#pragma once
using namespace std;

#define MaxSnakeSize 100
#define MaxFrameX 129          //default size console window
#define MaxFrameY 29

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoxy(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

class Point
{
private:
	int x;
	int y;
public:
	Point()
	{
		x = y = 10;
	}
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	void SetPoint(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	int GetX()
	{
		return x;
	}
	int GetY()
	{
		return y;
	}

	//movement
	void MoveUp()
	{
		y--;
		if (y < 0)
		{
			y = MaxFrameY;
		}
	}
	void MoveDown()
	{
		y++;
		if (y > MaxFrameY)
		{
			y = 0;
		}
	}
	void MoveLeft()
	{
		x--;
		if (x < 0)
		{
			x = MaxFrameX;
		}
	}
	void MoveRight()
	{
		x++;
		if (x > MaxFrameX)
		{
			x = 0;
		}
	}

	void Draw(char ch = 'O')
	{
		gotoxy(x, y);
		cout << ch;
	}
	void Erase()
	{
		gotoxy(x, y);
		cout << " ";
	}
	void CopyPos(Point* p)
	{
		p->x = x;
		p->y = y;
	}

	int IsEqual(Point* p)
	{
		if (p->x == x && p->y == y)
			return 1;
		return 0;
	}

};
