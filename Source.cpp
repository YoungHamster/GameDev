#include <iostream>
#include <string>
#include <random>
#include <Windows.h>
#include "GamePlay.h"
#include "MenuManager.h"


using namespace std;

/* Define code for stopping game*/
#define STOPGAME 123

/* Yet not defined names for keyboard input */
#define ENTER 5

/* Names for mouse input handling */
#define LEFT_BUTTON_CLICK  6
#define RIGHT_BUTTON_CLICK 7
#define WHEEL_CLICK		   8

bool volatile gamerunning = true;
unsigned int msPerMove = 300;

/* Prototypes of functions*/
// Generates random number
int randomNumber(int min, int max);

// Reads keyboard input and return it for following handling
int ReadInput();

// Moves snake
void move(Snake* snake, int* lastDir, int Dir);

/* All gameplay works there */
void GameLoop(Game* game, int startSnakeLength);

/* Menu works there */
void MenuLoop(Menu* menu);


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

	// Snake length, when game starts
	int startlength = 3;

	Game game(startlength, Current.w, Current.h);
	Menu menu(game.returnRendererHandle());
	while (gamerunning)
	{
		ReadInput();
		MenuLoop(&menu);
		if(gamerunning)
			GameLoop(&game, startlength);
	}
	// Stop rendering frames
	keeprendering = false;
	// Delay to stop rendering
	SDL_Delay(500);

	game.DeleteWindowAndQuitGame();
	cout << "YOU LOSE(or it was bug)" << endl;
	cin.get();

	return 0;
}

/* Gameplay works there */
void GameLoop(Game* game, int startSnakelength)
{
	gameplayrunning = true;

	int score = 0;
	//score = g.snakes[0].snakelength() - startlength;

	// Timer for reading input and moving snake
	unsigned long long currentTime = 0;
	unsigned long long lastMoveTime = 0;
	unsigned long long delta = 0;

	// Variables for input handling and moving snake
	int lastDir = LEFT;
	int Direction = LEFT;

	// Create apple
	Apple apple;
	apple.CreateApple(1, randomNumber(10, 30), randomNumber(5, 20));
	game->snakes[0].InitSnake(startSnakelength);

	// Create another thread for rendering
	thread RenderingThread(&Game::RenderFramesAsync, game, &game->snakes[0], &apple, &score);
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
		if (delta >= msPerMove)
		{
			// Move snake
			move(&game->snakes[0], &lastDir, Direction);

			// Update timer
			lastMoveTime = clock();

			// Check collisions between snake's head and borders/snake's body
			game->CheckAllCollisions(&game->snakes[0]);

			// Handle apple eating
			if (apple.CheckCollisionBetweenAppleAndSmth(game->snakes[0].getAABB(0)))
			{
				// Update score
				score++;
				game->snakes[0].addlength();
				apple.CreateApple(1, randomNumber(10, 30), randomNumber(5, 20));
				for (int i = 0; i < game->snakes[0].snakelength(); i++)
				{
					if (game->snakes[0].checkCollision(apple.ReturnBox(), i))
						apple.CreateApple(2, randomNumber(10, 30), randomNumber(5, 20));
				}
			}

			// Update score
			//score = g.snakes[0].snakelength() - startlength;
		}
	}
	gameplayrunning = false;
	SDL_Delay(500);
}

/* Menu loop */
void MenuLoop(Menu* menu)
{
	keeprendering = false;
	SDL_Delay(500);
	bool mainmenu = true;
	POINT cursor;
	int input;
	int CheckCollisions;

	/* Screen updates counters */
	unsigned long long currentTime = 0;
	unsigned long long lastFrameTime = 0;
	unsigned long long delta = 0;
	while (mainmenu)
	{
		input = ReadInput();
		GetCursorPos(&cursor);
		CheckCollisions = menu->CheckCollisions(cursor);
		switch (menu->GetCurrentPage())
		{
		// Main menu
		case MAIN_MENU:
			if (CheckCollisions == MAIN_START && input == LEFT_BUTTON_CLICK)
			{
				mainmenu = false;
			}
			if (CheckCollisions == MAIN_OPTIONS && input == LEFT_BUTTON_CLICK)
			{
				menu->ChangeCurrentPage(1);
				menu->RenderCurrentPage();
				SDL_Delay(500);
			}
			if (CheckCollisions == MAIN_MULTIPLAYER && input == LEFT_BUTTON_CLICK)
			{
				menu->ChangeCurrentPage(2);
				menu->RenderCurrentPage();
				SDL_Delay(500);
			}
			if (CheckCollisions == MAIN_EXIT && input == LEFT_BUTTON_CLICK)
			{
				gamerunning = false;
				mainmenu = false;
			}
		case OPTIONS:
			if (CheckCollisions == OPTIONS_MINUS && input == LEFT_BUTTON_CLICK)
			{
				--msPerMove;
				menu->RenderCurrentPage();
				menu->ChangeButtonTextAndStayInCenter("SPEED " + std::to_string(msPerMove + 1) + " MS PER MOVE", "SPEED " + std::to_string(msPerMove) + " MS PER MOVE");
				SDL_Delay(16);
			}
			if (CheckCollisions == OPTIONS_PLUS && input == LEFT_BUTTON_CLICK)
			{
				++msPerMove;
				menu->RenderCurrentPage();
				menu->ChangeButtonTextAndStayInCenter("SPEED " + std::to_string(msPerMove - 1) + " MS PER MOVE", "SPEED " + std::to_string(msPerMove) + " MS PER MOVE");
				SDL_Delay(16);
			}
			if (CheckCollisions == OPTIONS_BACK && input == LEFT_BUTTON_CLICK)
			{
				menu->ChangeCurrentPage(0);
				menu->RenderCurrentPage();
				SDL_Delay(500);
			}
		case MULTIPLAYER:
			if (CheckCollisions == MULTIPLAYER_BACK && input == LEFT_BUTTON_CLICK)
			{
				menu->ChangeCurrentPage(0);
				menu->RenderCurrentPage();
				SDL_Delay(500);
			}
		}
		if (input == STOPGAME)
			mainmenu = false;
		currentTime = clock();
		delta = currentTime - lastFrameTime;
		if (delta >= 33)
			menu->RenderCurrentPage();
	}
}

/* Read buttons input */
int ReadInput()
{
	/* Keyboard buttons handling */
	if (GetAsyncKeyState(0x57) == -32768 || GetAsyncKeyState(VK_UP) == -32768) return UP; // W key
	if (GetAsyncKeyState(0x41) == -32768 || GetAsyncKeyState(VK_LEFT) == -32768) return LEFT; // A key
	if (GetAsyncKeyState(0x53) == -32768 || GetAsyncKeyState(VK_DOWN) == -32768) return DOWN; // S key
	if (GetAsyncKeyState(0x44) == -32768 || GetAsyncKeyState(VK_RIGHT) == -32768) return RIGHT; // D key
	if (GetAsyncKeyState(VK_F4) == -32768 && GetAsyncKeyState(VK_MENU) == -32768) 
	{
		gamerunning = false;
		return STOPGAME;
	}
	if (GetAsyncKeyState(VK_F10) == -32768 && GetAsyncKeyState(VK_F1) == -32768) cout << "Easter Egg!!" << endl;

	if (GetAsyncKeyState(VK_RETURN) == -32768) return ENTER;

	/* Mouse buttons handling */
	if (GetAsyncKeyState(VK_LBUTTON) == -32768) return LEFT_BUTTON_CLICK;
	if (GetAsyncKeyState(VK_RBUTTON) == -32768) return RIGHT_BUTTON_CLICK;
	if (GetAsyncKeyState(VK_MBUTTON) == -32768) return WHEEL_CLICK;
	return 0;
}

/* Mouse states for main menu */
#define MOUSE_ON_START		 1
#define MOUSE_ON_OPTIONS	 2
#define MOUSE_ON_MULTIPLAYER 3
#define MOUSE_ON_EXIT		 4
#define CLICK_START			 5
#define CLICK_OPTIONS		 6
#define CLICK_MULTIPLAYER	 7
#define CLICK_EXIT			 8

/* Generates random numbet from min to max */
int randomNumber(int min, int max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

/* Moves snake */
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