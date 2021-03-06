#pragma once
#include "GamePhysics.h"
#include "GameRender.h"
#include "Snake.h"
#include "Apple.h"
#include <vector>
#include <time.h>

// synchronization between threads
bool volatile gameplayrunning = true;
bool volatile mainmenu = false;

class Game
{
private:
	GameRenderer GR;
public:
	//bool gameplayrunning = true;
	PhysicalObject borders[4]; // map borders
	Snake snakes[4]; // 4 snakes will be in multiplayeer
					 //std::vector<AABB> objects;
	Game(int length, int winW, int winH)
	{
		gameplayrunning = GR.InitGameRenderer(winW, winH);
		//Snake snake;
		//snake.InitSnake(length);
		//snakes.push_back(snake); // version used with vector of snakes
		//snakes[0] = snake;

		borders[0].type = BORDER;
		borders[0].borders.min.x = -2;
		borders[0].borders.min.y = -2;
		borders[0].borders.max.x = 1;
		borders[0].borders.max.y = 29;

		borders[1].type = BORDER;
		borders[1].borders.min.x = -2;
		borders[1].borders.min.y = 26;
		borders[1].borders.max.x = 50;
		borders[1].borders.max.y = 29;

		borders[2].type = BORDER;
		borders[2].borders.min.x = 47;
		borders[2].borders.min.y = -2;
		borders[2].borders.max.x = 50;
		borders[2].borders.max.y = 29;

		borders[3].type = BORDER;
		borders[3].borders.min.x = -2;
		borders[3].borders.min.y = -2;
		borders[3].borders.max.x = 50;
		borders[3].borders.max.y = 1;
	}

	// Check collisions between snake's head and borders, snake's body
	/*pointer on snake for synchronization between threads*/
	void CheckAllCollisions(Snake* snake)
	{
		for (int i = 0; i < 4; i++)
		{
			if (snakes[0].checkCollision(borders[i].borders, 0))
				gameplayrunning = false;
		}
		for (int i = 1; i < snakes[0].snakelength() - 1; i++)
		{
			if (snakes[0].checkCollisionSnakeHeadAndBody(i))
				gameplayrunning = false;
		}
	}

	// Renders GUI
	void RenderGUI(int score, int length)
	{
		GR.RenderGUI(score, length);
	}

	// Renders menu
	void RenderMenu(int page)
	{
		GR.RenderClear();
		GR.RenderBackground();
		GR.RenderMenu(page);
		GR.RenderPresent();
	}

	// Renders one frame
	void RenderFrame(PhysicalObject* apple)
	{
		GR.RenderClear();
		GR.RenderBackground();
		if (apple != NULL) {
			PhysicalObject tempApple;
			tempApple.type = APPLE;
			tempApple.borders = apple->borders;
			GR.RenderObject(tempApple);
		}
		// Renders borders
		for (int i = 0; i < 4; i++)
		{
			GR.RenderObject(borders[i]);
		}

		// Renders snake
		for (int i = 0; i < snakes[0].snakelength(); i++)
		{
			GR.RenderObject(snakes[0].getPhysicalObject(i));
		}
		GR.RenderPresent();
		//RenderGUI();
	}

	/*should be used in another thread!!!*/
	/*renders frames*/
	/*pointer on snake for synchronization between threads*/
	void RenderFramesAsync(Snake* snake, Apple* apple, int* score)
	{
		unsigned long long currentTime = 0;
		unsigned long long lastFrameTime = 0;
		unsigned long long delta = 0;
		while (gameplayrunning)
		{
			if (keeprendering) {
				currentTime = clock();
				delta = currentTime - lastFrameTime;

				// framerate == 60
				if (delta >= 16)
				{
					lastFrameTime = clock();
					RenderFrameAsync(snake, &apple->ReturnPhysicalObject(), *score);
				}
			}
		}
	}

	// Renders 1 frame
	/*pointer on snake for synchronization between threads*/
	void RenderFrameAsync(Snake* snake, PhysicalObject* apple, int score)
	{
		GR.RenderClear();
		GR.RenderBackground();
		if (apple != NULL) {
			PhysicalObject tempApple;
			tempApple.type = APPLE;
			tempApple.borders = apple->borders;
			GR.RenderObject(tempApple);
		}
		// Renders borders
		for (int i = 0; i < 4; i++)
		{
			GR.RenderObject(borders[i]);
		}

		// Renders snake
		for (int i = 0; i < snake->snakelength(); i++)
		{
			GR.RenderObject(snake->getPhysicalObject(i));
		}
		GR.RenderGUI(score, snake->snakelength());
		GR.RenderPresent();
	}

	// incapsulation
	void DeleteWindowAndQuitGame()
	{
		GR.DeleteWindowAndQuitGame();
	}

	GameRenderer* returnRendererHandle()
	{
		return &GR;
	}
};