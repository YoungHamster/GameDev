#include <iostream>
#include <string>
#include <random>
#include <Windows.h>
#include "GamePlay.h"


using namespace std;

/* Define code for stopping game*/
#define STOPGAME 123

/* Prototypes of runctions*/
// Generates random number
int randomNumber(int min, int max);

// Reads keyboard input and return it for following handling
int ReadInput();

// Moves snake
void move(Snake* snake, int* lastDir, int Dir);

 
int main(int argc, char * argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	IMG_Init(IMG_INIT_PNG);

	SDL_DisplayMode Current;
	SDL_GetCurrentDisplayMode(NULL, &Current);

	if (Current.w == 0 || Current.h == 0)
	{
		return -1;
	}

	// Screen width for renderer
	cout << Current.w << endl;

	// Screen height for renderer
	cout << Current.h << endl;

	// Snake lenght, when game starts
	int startLenght = 3;

	Game g(startLenght, Current.w, Current.h);

	int score = 0;
	//score = g.snakes[0].snakelenght() - startLenght;

	// Timer for reading input and moving snake
	unsigned long long currentTime = 0;
	unsigned long long lastMoveTime = 0;
	unsigned long long delta = 0;

	// Variables for input handling and moving snake
	int lastDir = LEFT;
	int Direction = LEFT;

	// Create apple
	Apple apl;
	apl.CreateApple(1, randomNumber(10, 30), randomNumber(5, 20));

	// Create another thread for rendering
	thread RenderingThread(&Game::RenderFramesAsync, g, &g.snakes[0], &apl, &score);
	RenderingThread.detach();

	SDL_Delay(500);

	while (gameplayrunning)
	{
		// Update timer
		currentTime = clock();
		delta = currentTime - lastMoveTime;

		/* Read input 60 times per second */
		if (delta >= 16)
		{
			int temp = ReadInput();
			switch (temp)
			{
			case STOPGAME: gameplayrunning = false; break;
			case UP: Direction = UP; break;
			case LEFT: Direction = LEFT; break;
			case DOWN: Direction = DOWN; break;
			case RIGHT: Direction = RIGHT; break;
			}
		}

		/* Move snake, check collisions and handle eating apples every 300 millisecond */
		if (delta >= 300)
		{
			// Move snake
			move(&g.snakes[0], &lastDir, Direction);

			// Update timer
			lastMoveTime = clock();

			// Check collisions between snake's head and borders/snake's body
			g.CheckAllCollisions(&g.snakes[0]);

			// Handle apple eating
			if (apl.CheckCollisionBetweenAppleAndSmth(g.snakes[0].getAABB(0)))
			{
				// Update score
				score++;
				g.snakes[0].addlenght();
				apl.CreateApple(1, randomNumber(10, 30), randomNumber(5, 20));
				for (int i = 0; i < g.snakes[0].snakelenght(); i++)
				{
					if (g.snakes[0].checkCollision(apl.ReturnBox(), i))
						apl.CreateApple(2, randomNumber(10, 30), randomNumber(5, 20));
				}
			}

			// Update score
			//score = g.snakes[0].snakelenght() - startLenght;
		}
	}
	// Stop rendering frames
	keeprendering = false;
	// Delay to stop rendering
	SDL_Delay(500);
	
	g.DeleteWindowAndQuitGame();
	cout << "YOU LOSE(or it was bug)" << endl;
	cin.get();
	
	return 0;
}

int ReadInput()
{
	if (GetAsyncKeyState(0x57) == -32768 || GetAsyncKeyState(VK_UP) == -32768) return UP; // W key
	if (GetAsyncKeyState(0x41) == -32768 || GetAsyncKeyState(VK_LEFT) == -32768) return LEFT; // A key
	if (GetAsyncKeyState(0x53) == -32768 || GetAsyncKeyState(VK_DOWN) == -32768) return DOWN; // S key
	if (GetAsyncKeyState(0x44) == -32768 || GetAsyncKeyState(VK_RIGHT) == -32768) return RIGHT; // D key
	if (GetAsyncKeyState(VK_F4) == -32768 && GetAsyncKeyState(VK_MENU) == -32768) return STOPGAME;
	if (GetAsyncKeyState(VK_F10) == -32768 && GetAsyncKeyState(VK_F1) == -32768) cout << "Easter Egg!!" << endl;
	return 0;
}

int randomNumber(int min, int max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

void move(Snake* snake, int* lastDir, int Dir)
{
	switch (Dir)
	{
	case UP:
		if (*lastDir != DOWN)
		{
			snake->move(UP);
			*lastDir = UP;
		}
		else snake->move(DOWN);
		break;
	case LEFT:
		if (*lastDir != RIGHT)
		{
			snake->move(LEFT);
			*lastDir = LEFT;
		}
		else snake->move(RIGHT);
		break;
	case DOWN:
		if (*lastDir != UP)
		{
			snake->move(DOWN);
			*lastDir = DOWN;
		}
		else snake->move(UP);
		break;
	case RIGHT:
		if (*lastDir != LEFT)
		{
			snake->move(RIGHT);
			*lastDir = RIGHT;
		}
		else snake->move(LEFT);
		break;
	default:
		snake->move(*lastDir);
		break;
	}
}