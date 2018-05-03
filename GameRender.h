#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <thread>
#include "GamePhysics.h"

/* Define positions in array of rects */
#define SRC_RECT_SNAKE_HEAD_UP		0
#define SRC_RECT_SNAKE_HEAD_LEFT	1
#define SRC_RECT_SNAKE_HEAD_DOWN	2
#define SRC_RECT_SNAKE_HEAD_RIGHT	3
#define SRC_RECT_SNAKE_BODY			4
#define SRC_RECT_SNAKE_TAIL_UP		5
#define SRC_RECT_SNAKE_TAIL_LEFT	6
#define SRC_RECT_SNAKE_TAIL_DOWN	7
#define SRC_RECT_SNAKE_TAIL_RIGHT	8
#define SRC_RECT_SCORE				9
#define SRC_RECT_SNAKE_LENGHT		20
#define SRC_RECT_MAIN_MENU			21
#define SRC_RECT_START				22
#define SRC_RECT_OPTIONS			23

/* Define textures number */
#define BACKGROUND_TEXTURE	0
#define BORDER_TEXTURE		1
#define SNAKE_TEXTURE		2
#define APPLE_TEXTURE		3
#define TEXTS_TEXTURE		4

/* Define main menu pages */
#define MAIN_MENU
#define OPTIONS
#define MULTIPLAYER // For future

int ReturnDischargeOfNumber(int number, int discharge)
{
	if (discharge > number)
		return 0;
	if (number == discharge)
		return number;
	if (number < discharge * 10)
		return number - number % discharge;
	else
		return number - number % discharge - (number - number % (discharge * 10));
	return -2;
}

class GameRenderer
{
private:
	int winH; // Window hegiht
	int winW; // Window width
	double pixOnBlock; // Pixels on 1 game distance unit
	SDL_Texture* textures[5]; // Array of pointers on textures
	SDL_Rect srcRects[24]; // Array of rects for cutting it from source textures
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Rect rect; // Variable for function RenderObject
public:
	bool InitGameRenderer(int windowW, int windowH)
	{
		winW = windowW;
		winH = windowH;
		pixOnBlock = winW / 48.0;

		window = SDL_CreateWindow("SnakeGame", 0, 0, winW, winH, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
		if (window == NULL)
			return false;

		// renderer with Vertical Synchronization
		//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		// renderer without Vertical Synchronization
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
			return false;

		for (int i = 0; i < 4; i++)
		{
			srcRects[i].x = i*30;
			srcRects[i].y = 0;
			srcRects[i].w = 30;
			srcRects[i].h = 30;
		}
		srcRects[SRC_RECT_SNAKE_BODY].x = 0;
		srcRects[SRC_RECT_SNAKE_BODY].y = 30;
		srcRects[SRC_RECT_SNAKE_BODY].w = 30;
		srcRects[SRC_RECT_SNAKE_BODY].h = 30;
		for (int i = 0; i < 4; i++)
		{
			srcRects[i+5].x = i * 30;
			srcRects[i+5].y = 60;
			srcRects[i+5].w = 30;
			srcRects[i+5].h = 30;
		}


		// YOUR SCORE
		srcRects[9].x = 0;
		srcRects[9].y = 0;
		srcRects[9].w = 62;
		srcRects[9].h = 8;

		// 0
		srcRects[10].x = 56;
		srcRects[10].y = 19;
		srcRects[10].w = 6;
		srcRects[10].h = 8;

		// 1
		srcRects[11].x = 1;
		srcRects[11].y = 19;
		srcRects[11].w = 3;
		srcRects[11].h = 8;

		// 2
		srcRects[12].x = 5;
		srcRects[12].y = 19;
		srcRects[12].w = 5;
		srcRects[12].h = 8;

		// 3
		srcRects[13].x = 11;
		srcRects[13].y = 19;
		srcRects[13].w = 5;
		srcRects[13].h = 8;

		// 4
		srcRects[14].x = 17;
		srcRects[14].y = 19;
		srcRects[14].w = 6;
		srcRects[14].h = 8;

		// 5
		srcRects[15].x = 24;
		srcRects[15].y = 19;
		srcRects[15].w = 6;
		srcRects[15].h = 8;

		// 6
		srcRects[16].x = 31;
		srcRects[16].y = 19;
		srcRects[16].w = 5;
		srcRects[16].h = 8;

		// 7
		srcRects[17].x = 37;
		srcRects[17].y = 19;
		srcRects[17].w = 5;
		srcRects[17].h = 8;

		// 8
		srcRects[18].x = 43;
		srcRects[18].y = 19;
		srcRects[18].w = 6;
		srcRects[18].h = 8;

		// 9
		srcRects[19].x = 50;
		srcRects[19].y = 19;
		srcRects[19].w = 5;
		srcRects[19].h = 8;

		//Reserved for SNAKE LENGHT
		//srcRects[20]
		
		srcRects[SRC_RECT_MAIN_MENU].x = 0;
		srcRects[SRC_RECT_MAIN_MENU].y = 28;
		srcRects[SRC_RECT_MAIN_MENU].w = 58;
		srcRects[SRC_RECT_MAIN_MENU].h = 8;

		srcRects[SRC_RECT_START].x = 0;
		srcRects[SRC_RECT_START].y = 37;
		srcRects[SRC_RECT_START].w = 33;
		srcRects[SRC_RECT_START].h = 8;

		srcRects[SRC_RECT_OPTIONS].x = 0;
		srcRects[SRC_RECT_OPTIONS].y = 46;
		srcRects[SRC_RECT_OPTIONS].w = 42;
		srcRects[SRC_RECT_OPTIONS].h = 8;

		std::string filename = "Background.png";
		textures[BACKGROUND_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[BACKGROUND_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}
		
		filename = "Border.png";
		textures[BORDER_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[BORDER_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}

		filename = "GreenSnake.png";
		textures[SNAKE_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[SNAKE_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}

		filename = "Apple.png";
		textures[APPLE_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[APPLE_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}

		filename = "TextsForGUI.png";
		textures[TEXTS_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[TEXTS_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}

		return true;
	}
	

	void RenderObject(PhysicalObject obj)
	{
		rect.h = (obj.borders.max.y - obj.borders.min.y) * pixOnBlock;
		rect.w = (obj.borders.max.x - obj.borders.min.x) * pixOnBlock;
		rect.x = obj.borders.min.x * pixOnBlock;
		rect.y = winH - obj.borders.max.y * pixOnBlock;

		switch (obj.type)
		{
			//Snake parts cases
		case SNAKE_HEAD_UP: 
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SRC_RECT_SNAKE_HEAD_UP], &rect); break;
		case SNAKE_HEAD_LEFT:
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SRC_RECT_SNAKE_HEAD_LEFT], &rect);break;
		case SNAKE_HEAD_DOWN:
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SRC_RECT_SNAKE_HEAD_DOWN], &rect); break;
		case SNAKE_HEAD_RIGHT:
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SRC_RECT_SNAKE_HEAD_RIGHT], &rect);
			break;

		case SNAKE_BODY: SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SRC_RECT_SNAKE_BODY], &rect); break;

		case SNAKE_TAIL_UP: 
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SRC_RECT_SNAKE_TAIL_UP], &rect); break;
		case SNAKE_TAIL_RIGHT:
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SRC_RECT_SNAKE_TAIL_LEFT], &rect); break;
		case SNAKE_TAIL_DOWN: 
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SRC_RECT_SNAKE_TAIL_DOWN], &rect); break;
		case SNAKE_TAIL_LEFT: 
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SRC_RECT_SNAKE_TAIL_RIGHT], &rect); break;
		
			// Apple case
		case APPLE: SDL_RenderCopy(renderer, textures[APPLE_TEXTURE], NULL, &rect); break;
			// Border case
		case BORDER: SDL_RenderCopy(renderer, textures[BORDER_TEXTURE], NULL, &rect); break;
		}
	}

	void RenderMainMenu(int page)
	{
		switch (page)
		{

		}
	}

	void RenderGUI(int score, int snakeLenght)
	{
		SDL_Rect rect;
		rect.x = winW - pixOnBlock * 15;
		rect.y = pixOnBlock * 2;
		rect.w = 62 * (pixOnBlock / 7);
		rect.h = 8 * (pixOnBlock / 7);
		SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[SRC_RECT_SCORE], &rect);

		rect.x = rect.x + rect.w;

		// Render scroe from 0 to 9
		if (score < 10) {
			if (score == 1)
				rect.w = 3 * (pixOnBlock / 7);
			else
				rect.w = 6 * (pixOnBlock / 7);
			rect.x = rect.x + 3 * (pixOnBlock / 7);
			SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + score], &rect);
		}
		else {
			// Render score from 10 to 99
			if (score < 100) {
				if (ReturnDischargeOfNumber(score, 10) == 10)
					rect.w = 3 * (pixOnBlock / 7);
				else
					rect.w = 6 * (pixOnBlock / 7);
				rect.x = rect.x + rect.w + pixOnBlock / 7;
				SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + ReturnDischargeOfNumber(score, 10) / 10], &rect);
				if (ReturnDischargeOfNumber(score, 1) == 1)
					rect.w = 3 * (pixOnBlock / 7);
				else
					rect.w = 6 * (pixOnBlock / 7);
				rect.x = rect.x + rect.w + pixOnBlock / 7;
				SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + ReturnDischargeOfNumber(score, 1)], &rect);
			}
			else {
				// Render score from 100 to 999
				if (score < 1000) {
					if (ReturnDischargeOfNumber(score, 100) == 100)
						rect.w = 3 * (pixOnBlock / 7);
					else
						rect.w = 6 * (pixOnBlock / 7);
					rect.x = rect.x + rect.w + pixOnBlock / 7;
					SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + ReturnDischargeOfNumber(score, 100) / 100], &rect);
					if (ReturnDischargeOfNumber(score, 10) == 10)
						rect.w = 3 * (pixOnBlock / 7);
					else
						rect.w = 6 * (pixOnBlock / 7);
					rect.x = rect.x + rect.w + pixOnBlock / 7;
					SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + ReturnDischargeOfNumber(score, 10) / 10], &rect);
					if (ReturnDischargeOfNumber(score, 1) == 1)
						rect.w = 3 * (pixOnBlock / 7);
					else
						rect.w = 6 * (pixOnBlock / 7);
					rect.x = rect.x + rect.w + pixOnBlock / 7;
					SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + ReturnDischargeOfNumber(score, 1) / 1], &rect);
				}
				// If score bigger then 999 write to console "Wrong score!"
				else
					std::cout << "Error! Wrong score!" << std::endl;
			}
		}
	}

	void RenderBackground()
	{
		SDL_Rect rect;
		rect.h = winH;
		rect.w = winW;
		rect.x = 0;
		rect.y = 0;
		SDL_RenderCopy(renderer, textures[BACKGROUND_TEXTURE], NULL, &rect);
	}

	void RenderCustomTexture(SDL_Texture* tex, SDL_Rect srcrect, SDL_Rect rect)
	{
		SDL_RenderCopy(renderer, tex, &srcrect, &rect);
	}

	void RenderCLear()
	{
		SDL_RenderClear(renderer);
	}

	void RenderPresent()
	{
		SDL_RenderPresent(renderer);
	}

	void DeleteWindowAndQuitGame()
	{
		for (int i = 0; i < 4; i++)
		{
			SDL_DestroyTexture(textures[i]);
		}
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
	}
};