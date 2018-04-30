#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <thread>
#include "GamePhysics.h"

#define SNAKE_HEAD_UP_RECT 0
#define SNAKE_HEAD_LEFT_RECT 1
#define SNAKE_HEAD_DOWN_RECT 2
#define SNAKE_HEAD_RIGHT_RECT 3
#define SNAKE_BODY_RECT 4
#define SNAKE_TAIL_UP_RECT 5
#define SNAKE_TAIL_LEFT_RECT 6
#define SNAKE_TAIL_DOWN_RECT 7
#define SNAKE_TAIL_RIGHT_RECT 8
#define SCORE_RECT 9
#define SNAKE_LENGHT_RECT 10

#define BACKGROUND_TEXTURE 0
#define BORDER_TEXTURE 1
#define SNAKE_TEXTURE 2
#define APPLE_TEXTURE 3
#define TEXTS_TEXTURE 4

int ReturnDischargeOfNumber(int number, int discharge)
{
	if (discharge > number)
		return 0;
	if (number == discharge)
		return 1;
	if (number < discharge * 10)
		return number - number % discharge;
	else
		return number - number % discharge - (number - number % (discharge * 10));
	return -2;
}

class GameRenderer
{
private:
	int winH;
	int winW;
	double pixOnBlock;
	SDL_Texture* textures[5];
	SDL_Rect srcRects[21];
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Rect rect;
	SDL_Rect srcRect;
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
		srcRects[SNAKE_BODY_RECT].x = 0;
		srcRects[SNAKE_BODY_RECT].y = 30;
		srcRects[SNAKE_BODY_RECT].w = 30;
		srcRects[SNAKE_BODY_RECT].h = 30;
		for (int i = 0; i < 4; i++)
		{
			srcRects[i+5].x = i * 30;
			srcRects[i+5].y = 60;
			srcRects[i+5].w = 30;
			srcRects[i+5].h = 30;
		}

		srcRects[9].x = 0;
		srcRects[9].y = 0;
		srcRects[9].w = 62;
		srcRects[9].h = 8;

		srcRects[10].x = 0;
		srcRects[10].y = 9;
		srcRects[10].w = 75;
		srcRects[10].h = 16;

		srcRects[11].x = 0;
		srcRects[11].y = 19;
		srcRects[11].w = 4;
		srcRects[11].h = 26;

		for (int i = 0; i < 9; i++)
		{
			srcRects[12 + i].x = i * 5 + 4;
			srcRects[12 + i].y = 19;
			srcRects[12 + i].w = 6;
			srcRects[12 + i].h = 9;
		}

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
		case SNAKE_HEAD_UP: 
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SNAKE_HEAD_UP_RECT], &rect); break;
		case SNAKE_HEAD_LEFT:
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SNAKE_HEAD_LEFT_RECT], &rect);break;
		case SNAKE_HEAD_DOWN:
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SNAKE_HEAD_DOWN_RECT], &rect); break;
		case SNAKE_HEAD_RIGHT:
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SNAKE_HEAD_RIGHT_RECT], &rect);
			break;

		case SNAKE_BODY: SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SNAKE_BODY_RECT], &rect); break;

		case SNAKE_TAIL_UP: 
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SNAKE_TAIL_UP_RECT], &rect); break;
		case SNAKE_TAIL_RIGHT:
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SNAKE_TAIL_LEFT_RECT], &rect); break;
		case SNAKE_TAIL_DOWN: 
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SNAKE_TAIL_DOWN_RECT], &rect); break;
		case SNAKE_TAIL_LEFT: 
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SNAKE_TAIL_RIGHT_RECT], &rect); break;
		

		case APPLE: SDL_RenderCopy(renderer, textures[APPLE_TEXTURE], NULL, &rect); break;
		case BORDER: SDL_RenderCopy(renderer, textures[BORDER_TEXTURE], NULL, &rect); break;
		}
	}

	void RenderGUI(int score, int snakeLenght)
	{
		SDL_Rect rect;
		int counter = 3;
		rect.x = winW - pixOnBlock * 15;
		rect.y = pixOnBlock * 2;
		rect.w = 62 * counter;
		rect.h = 8 * counter;
		SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[SCORE_RECT], &rect);

		rect.x = rect.x + rect.w;

		if (score < 10) {
			if (score == 1)
				rect.w = 5 * counter;
			else
				rect.w = 6 * counter;
			rect.x = rect.x + 3 * counter;
			SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + score], &rect);
		}
		else {
			if (score < 100) {
				if (ReturnDischargeOfNumber(score, 10) == 100)
					rect.w = 5 * counter;
				else
					rect.w = 6 * counter;
				rect.x = rect.x + rect.w + 3 * counter;
				SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + ReturnDischargeOfNumber(score, 10) / 10], &rect);
				if (ReturnDischargeOfNumber(score, 1) == 1)
					rect.w = 5 * counter;
				else
					rect.w = 6 * counter;
				rect.x = rect.x + rect.w + 3 * counter;
				SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + ReturnDischargeOfNumber(score, 1)], &rect);
			}
			else {
				if (score < 1000) {
					if (ReturnDischargeOfNumber(score, 100) == 100)
						rect.w = 5 * counter;
					else
						rect.w = 6 * counter;
					rect.x = rect.x + rect.w + 3 * counter;
					SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + ReturnDischargeOfNumber(score, 100) / 100], &rect);
					if (ReturnDischargeOfNumber(score, 10) == 10)
						rect.w = 4 * counter;
					else
						rect.w = 5 * counter;
					rect.x = rect.x + rect.w + 3 * counter;
					SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + ReturnDischargeOfNumber(score, 10) / 10], &rect);
					if (ReturnDischargeOfNumber(score, 1) == 1)
						rect.w = 5 * counter;
					else
						rect.w = 6 * counter;
					rect.x = rect.x + rect.w + 3 * counter;
					SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[10 + ReturnDischargeOfNumber(score, 1) / 1], &rect);
				}
				else
					std::cout << "Error! Wrong score!" << std::endl;
			}
		}

		rect.x = winW - 13 - pixOnBlock * 5;
		rect.y = rect.y - pixOnBlock * 1.1;
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