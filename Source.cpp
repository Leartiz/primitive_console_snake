/*
* ==========================================================================
* /File: Source.cpp /
* /Brief: Realization of the game /
* /Creator: Leonid Artemev | Leartiz /
* /Date: 2018 /
* ==========================================================================
*/

#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <clocale>
#include <Windows.h>

#define WIDTH 24 // - Width map
#define HEIGHT 18 // | Height map 

// Start
int difficult();
void instruction();

void initialization_map(char(&map)[HEIGHT][WIDTH]);

// Output
void out_map_console(int *tail, const int &score, const int &amount,
	char(&map)[HEIGHT][WIDTH], const int& xfoo, const int& yfoo);
void out_array(int *tail, int const& amount);

// Way
enum class Control { STOP = 0, W, S, A, D };

// Snake
void motion(int *tail, Control& step);
void route(Control& step);
void rewrite_array(int *tail, const int& amount,
	char(&map)[HEIGHT][WIDTH], const int& xfoo, const int& yfoo);
void knock_wall(int *tail, bool& end);
void eat_food(int *tail, int & score, int &amount, int& xfoo, int& yfoo);
void ad_tail(int *tail, const int& amount, const int& xbuf, const int& ybuf);
void tail_motion(int *tail, const int& amount);

// System
void cls();
void pause_end(bool& end);

int main()
{
	// Console window
	HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD crd;
	crd.X = WIDTH + 25;
	crd.Y = HEIGHT + 15;
	SMALL_RECT src = { 0, 0, crd.X - 1, crd.Y - 1 };
	SetConsoleWindowInfo(out_handle, true, &src);
	SetConsoleScreenBufferSize(out_handle, crd);
	SetConsoleTitle("Snake 0.1"); // Name

	char map[HEIGHT][WIDTH]; // Map

	Control step = Control::STOP;

	bool end = false;
	int score = 0;

	int tail[WIDTH * HEIGHT]; // Body
	int amount = 0; // + To body

	// Head
	tail[0] = WIDTH / 2;
	tail[1] = HEIGHT / 2;

	// Food Coordinates
	int xfoo = (rand() % (WIDTH - 2)) + 1;
	int yfoo = (rand() % (HEIGHT - 2)) + 1;

	// Output of the instruction
	instruction();

	// Filling in the map
	initialization_map(map);

	// Choice of difficulty level
	// Difficulty level
	int diff_lvl = difficult();

	while (!end)
	{
		out_map_console(tail, score, amount, map, xfoo, yfoo); // Output

		motion(tail, step); // Constant motion
		route(step);
			pause_end(end); // ...
		knock_wall(tail, end);
		eat_food(tail, score, amount, xfoo, yfoo);
		rewrite_array(tail, amount, map, xfoo, yfoo);
		tail_motion(tail, amount);

		Sleep(diff_lvl);
		cls();
	}

	_getch();

	return 0;
}

void instruction()
{
	printf("\"Instruction\" \n\n");
	printf("W - UP \n");
	printf("S - DOWN \n");
	printf("A - LEFT \n");
	printf("D - RIGHT \n");
	printf("To pause use ESC \n");
	printf("\n\n");
	_getch();

	system("CLS");
}

int difficult()
{
	int diff_lvl = 0;
	std::cout << "Difficulty level: \n\n";
	std::cout << "1. Very difficult \n";
	std::cout << "2. Difficult \n";
	std::cout << "3. Average \n";
	std::cout << "4. Easy \n";
	std::cin >> diff_lvl;
	diff_lvl = diff_lvl * 20;

	system("CLS");

	return diff_lvl;
}


void initialization_map(char(&map)[HEIGHT][WIDTH])
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1)
				map[i][j] = '#';
			else
				map[i][j] = ' ';
		}
	}
}

void out_map_console(int *tail, const int &score, const int &amount,
	char(&map)[HEIGHT][WIDTH], const int& xfoo, const int& yfoo)
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			std::cout << map[i][j];
		}
		std::cout << std::endl;
	}

	std::cout << std::endl << "Score = " << score << std::endl;
	std::cout << std::endl << "Amount = " << amount << std::endl;
	std::cout << "Head: \n";
	std::cout << "X =   \b\b" << tail[0] << std::endl;
	std::cout << "Y =   \b\b" << tail[1] << std::endl;;
	std::cout << "Food: \n";
	std::cout << "X =   \b\b" << xfoo << std::endl;
	std::cout << "Y =   \b\b" << yfoo << std::endl;
}

void route(Control &step)
{
	if (_kbhit())
	{
		int key = _getch();
		switch (key)
		{
		case 'W': case 'w': case 72:
			step = Control::W;
			break;
		case 'S': case 's': case 80:
			step = Control::S;
			break;
		case 'D': case 'd': case 77:
			step = Control::D;
			break;
		case 'A': case 'a': case 75:
			step = Control::A;
			break;
		case 27:
			_ungetch(key);
			return;
			break;
		}
	}
}

void pause_end(bool& end)
{
	if (_kbhit())
	{
		int key = _getch();
		switch (key)
		{
		case 27:
			std::cout << "\n --- PAUSE --- \n";
			std::cout << "Continue = 1 \n";
			std::cout << "Exit = any \n";

			std::cin >> key;
			if (key != 1)
				end = true; // Exit / End

			system("CLS");
			break;
		default:
			break;
		}
	}
}


void motion(int *tail, Control & step)
{
	switch (step)
	{
	case Control::W:
		--tail[1];
		break;
	case Control::S:
		++tail[1];
		break;
	case Control::A:
		--tail[0];
		break;
	case Control::D:
		++tail[0];
		break;
	}
}

void rewrite_array(int *tail, const int& amount,
	char(&map)[HEIGHT][WIDTH], const int& xfoo, const int& yfoo)
{
	// Editing without borders
	for (int i = 1; i < HEIGHT - 1; ++i)
	{
		for (int j = 1; j < WIDTH - 1; ++j)
		{
			if (i == yfoo && j == xfoo)
			{
				map[i][j] = 'F';
			}
			else if (i == tail[1] && j == tail[0])
			{
				map[i][j] = '%';
			}
			else
			{
				map[i][j] = ' ';
				if (amount > 0)
				{
					for (int o = 2; o < amount * 2 + 1; o += 2)
					{
						map[tail[o + 1]][tail[o]] = 'o';
					}
				}
			}
		}
	}
}

void knock_wall(int *tail, bool& end)
{
	// Without impact
	//if (tail[0] == 0) ++tail[0];
	//if (tail[0] == WIDTH - 1) --tail[0];
	//if (tail[1] == 0) ++tail[1];
	//if (tail[1] == HEIGHT - 1) --tail[1];

	// Through the wall
	if (tail[0] == 0) tail[0] = WIDTH - 2;
	if (tail[0] == WIDTH - 1) tail[0] = 1;
	if (tail[1] == 0) tail[1] = HEIGHT - 2;
	if (tail[1] == HEIGHT - 1) tail[1] = 1;

	// End of the game
	//if (tail[0] == 0) end = true;
	//if (tail[0] == WIDTH - 1) end = true;
	//if (tail[1] == 0) end = true;
	//if (tail[1] == HEIGHT - 1) end = true;
}

void eat_food(int *tail, int & score, int &amount, int& xfoo, int& yfoo)
{
	int xbuf = 0;
	int ybuf = 0;

	if (tail[0] == xfoo && tail[1] == yfoo)
	{
		amount++;
		xbuf = xfoo;
		ybuf = yfoo;
		xfoo = (rand() % (WIDTH - 2)) + 1;
		yfoo = (rand() % (HEIGHT - 2)) + 1;
		score += 5;
		ad_tail(tail, amount, xbuf, ybuf);
	}
}

// A record of coordinates where a fruit is eaten, into a one-dimensional array
void ad_tail(int *tail, const int& amount, const int& xbuf, const int& ybuf)
{
	for (int i = amount * 2 + 1; i > 1; i -= 2)
	{
		tail[i] = tail[i - 2];
		tail[i - 1] = tail[i - 3];
	}

	tail[2] = xbuf;
	tail[3] = ybuf;
}

void out_array(int *tail, int const& amount)
{
	for (int i = 0; i < amount * 2 + 1; i += 2)
	{
		std::cout << "x = " << tail[i] << " y = " << tail[i + 1];
		std::cout << std::endl;
	}
}

void tail_motion(int *tail, const int& amount)
{
	for (int i = amount * 2 + 1; i > 1; i -= 2)
	{
		tail[i] = tail[i - 2];
		tail[i - 1] = tail[i - 3];
	}
}

void cls()
{
	HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cd;
	cd.X = 0;
	cd.Y = 0;
	SetConsoleCursorPosition(hd, cd);
}
