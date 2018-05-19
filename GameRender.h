#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <thread>
#include "GamePhysics.h"

/* Define positions in array of rects */
/* Snake rects */
#define SRC_RECT_SNAKE_HEAD_UP		0
#define SRC_RECT_SNAKE_HEAD_LEFT	1
#define SRC_RECT_SNAKE_HEAD_DOWN	2
#define SRC_RECT_SNAKE_HEAD_RIGHT	3
#define SRC_RECT_SNAKE_BODY			4
#define SRC_RECT_SNAKE_TAIL_UP		5
#define SRC_RECT_SNAKE_TAIL_LEFT	6
#define SRC_RECT_SNAKE_TAIL_DOWN	7
#define SRC_RECT_SNAKE_TAIL_RIGHT	8
/* Text rects */
#define SRC_RECT_SCORE				9
#define SRC_RECT_SNAKE_LENGTH		20

/* Alphabet rects */
#define A 21
#define B 22
#define C 23
#define D 24
#define E 25
#define F 26
#define G 27
#define H 28
#define I 29
#define J 30
#define K 31
#define L 32
#define M 33
#define N 34
#define O 35
#define P 36
#define Q 37
#define R 38
#define S 39
#define T 40
#define U 41
#define V 42
#define W 43
#define X 44
#define Y 45
#define Z 46
#define SPACE 47
#define SLASH 48
#define PLUS 49
#define MINUS 50

/* Define textures number */
#define BACKGROUND_TEXTURE			0
#define BORDER_TEXTURE				1
#define SNAKE_TEXTURE				2
#define APPLE_TEXTURE				3
#define TEXTS_TEXTURE				4
#define ALPHABET_TEXTURE			5
#define BUTTON_BACKGROUND_TEXTURE	6

/* Define main menu pages */
#define MAIN_MENU_START		  1
#define MAIN_MENU_OPTIONS	  2
#define MAIN_MENU_MULTIPLAYER 3
#define MAIN_MENU_EXIT		  4
#define OPTIONS				  5
#define MULTIPLAYER			  6 // For future

const int sizeOftexturesArray = 7;
const int sizeOfsourceRectsArray = 51;

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
	SDL_Texture* textures[sizeOftexturesArray]; // Array of pointers on textures
	SDL_Rect srcRects[sizeOfsourceRectsArray]; // Array of rects for cutting it from source textures
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Rect rect; // Variable for function RenderObject

	void WriteRect(SDL_Rect* rect, int x, int y, int w, int h)
	{
		rect->x = x;
		rect->y = y;
		rect->w = w;
		rect->h = h;
	}
public:
	bool InitGameRenderer(int windowW, int windowH)
	{
		winW = windowW;
		winH = windowH;
		pixOnBlock = winW / 48.0;

		window = SDL_CreateWindow("SnakeGame", 0, 0, winW, winH, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
		//window = SDL_CreateWindow("SnakeGame", 0, 0, winW, winH, SDL_WINDOW_SHOWN);
		if (window == NULL)
			return false;

		// renderer with Vertical Synchronization
		//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		// renderer without Vertical Synchronization
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
			return false;

		/* Creating many many source rectangles */
		for (int i = 0; i < 4; i++)
		{
			srcRects[i].x = i * 30;
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
			srcRects[i + 5].x = i * 30;
			srcRects[i + 5].y = 60;
			srcRects[i + 5].w = 30;
			srcRects[i + 5].h = 30;
		}


		// YOUR SCORE text
		srcRects[9].x = 0;
		srcRects[9].y = 0;
		srcRects[9].w = 63;
		srcRects[9].h = 8;

		/* Numbers 0-9 */
		WriteRect(&srcRects[10], 55, 37, 7, 8);
		WriteRect(&srcRects[11], 0, 37, 4, 8);
		WriteRect(&srcRects[12], 4, 37, 6, 8);
		WriteRect(&srcRects[13], 10, 37, 6, 8);
		WriteRect(&srcRects[14], 16, 37, 7, 8);
		WriteRect(&srcRects[15], 23, 37, 7, 8);
		WriteRect(&srcRects[16], 30, 37, 6, 8);
		WriteRect(&srcRects[17], 36, 37, 6, 8);
		WriteRect(&srcRects[18], 42, 37, 7, 8);
		WriteRect(&srcRects[19], 49, 37, 6, 8);

		//Reserved for SNAKE length text
		//srcRects[20]

		/* Alphabet source rects in alphabet.png */
		WriteRect(&srcRects[A], 0, 0, 8, 8);
		WriteRect(&srcRects[B], 8, 0, 7, 8);
		WriteRect(&srcRects[C], 15, 0, 6, 8);
		WriteRect(&srcRects[D], 21, 0, 7, 8);
		WriteRect(&srcRects[E], 28, 0, 6, 8);
		WriteRect(&srcRects[F], 34, 0, 6, 8);
		WriteRect(&srcRects[G], 40, 0, 8, 8);
		WriteRect(&srcRects[H], 48, 0, 7, 8);
		WriteRect(&srcRects[I], 55, 0, 5, 8);
		WriteRect(&srcRects[J], 0, 9, 6, 8);
		WriteRect(&srcRects[K], 6, 9, 6, 8);
		WriteRect(&srcRects[L], 12, 9, 6, 8);
		WriteRect(&srcRects[M], 18, 9, 9, 8);
		WriteRect(&srcRects[N], 27, 9, 9, 8);
		WriteRect(&srcRects[O], 36, 9, 7, 8);
		WriteRect(&srcRects[P], 43, 9, 7, 8);
		WriteRect(&srcRects[Q], 50, 9, 7, 10);
		WriteRect(&srcRects[R], 0, 19, 7, 8);
		WriteRect(&srcRects[S], 7, 19, 6, 8);
		WriteRect(&srcRects[T], 13, 19, 7, 8);
		WriteRect(&srcRects[U], 20, 19, 7, 8);
		WriteRect(&srcRects[V], 27, 19, 8, 8);
		WriteRect(&srcRects[W], 35, 19, 13, 8);
		WriteRect(&srcRects[X], 48, 19, 8, 8);
		WriteRect(&srcRects[Y], 0, 28, 7, 8);
		WriteRect(&srcRects[Z], 7, 28, 7, 8);
		WriteRect(&srcRects[SPACE], 13, 28, 5, 8);
		WriteRect(&srcRects[SLASH], 23, 28, 8, 8);
		WriteRect(&srcRects[PLUS], 31, 28, 8, 8);
		WriteRect(&srcRects[MINUS], 40, 28, 7, 8);


		/* Loading textures */
		std::string filename = "textures\\Background.png";
		textures[BACKGROUND_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[BACKGROUND_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}

		filename = "textures\\Border.png";
		textures[BORDER_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[BORDER_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}

		filename = "textures\\GreenSnake.png";
		textures[SNAKE_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[SNAKE_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}

		filename = "textures\\Apple.png";
		textures[APPLE_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[APPLE_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}

		filename = "textures\\TextsForGUI.png";
		textures[TEXTS_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[TEXTS_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}

		filename = "textures\\Alphabet.png";
		textures[ALPHABET_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[ALPHABET_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}

		filename = "textures\\ButtonBackground.png";
		textures[BUTTON_BACKGROUND_TEXTURE] = IMG_LoadTexture(renderer, filename.c_str());
		if (textures[BUTTON_BACKGROUND_TEXTURE] == NULL) {
			std::cout << "ERROR!!! Can not load texture " << filename << std::endl;
			return false;
		}

		return true;
	}

	void RenderMenu(int page)
	{
		
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
			SDL_RenderCopy(renderer, textures[SNAKE_TEXTURE], &srcRects[SRC_RECT_SNAKE_HEAD_LEFT], &rect); break;
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

	void RenderGUI(int score, int snakelength)
	{
		int dischargeOfRenderingNumber;
		SDL_Rect rect;
		rect.x = winW - pixOnBlock * 15;
		rect.y = pixOnBlock * 2;
		rect.w = 62 * (pixOnBlock / 7);
		rect.h = 8 * (pixOnBlock / 7);
		SDL_RenderCopy(renderer, textures[TEXTS_TEXTURE], &srcRects[SRC_RECT_SCORE], &rect);
		rect.x = rect.x + (pixOnBlock / 7) * 3;
		/* Render scroe from 0 to 9 */
		if (score < 10) {
			rect.x = rect.x + rect.w;
			rect.w = srcRects[10 + score].w * (pixOnBlock / 7);
			SDL_RenderCopy(renderer, textures[ALPHABET_TEXTURE], &srcRects[10 + score], &rect);
		}
		else {
			/* Render score from 10 to 99 */
			if (score < 100) {
				/* Render tens */
				dischargeOfRenderingNumber = ReturnDischargeOfNumber(score, 10) / 10;
				rect.x = rect.x + rect.w;
				rect.w = srcRects[10 + dischargeOfRenderingNumber].w * (pixOnBlock / 7);
				SDL_RenderCopy(renderer, textures[ALPHABET_TEXTURE], &srcRects[10 + dischargeOfRenderingNumber], &rect);
				/* Render ones */
				dischargeOfRenderingNumber = ReturnDischargeOfNumber(score, 1);
				rect.x = rect.x + rect.w;
				rect.w = srcRects[10 + dischargeOfRenderingNumber].w * (pixOnBlock / 7);
				SDL_RenderCopy(renderer, textures[ALPHABET_TEXTURE], &srcRects[10 + dischargeOfRenderingNumber], &rect);
			}
			else {
				/* Render score from 100 to 999 */
				if (score < 1000) {
					/* Render hundreds */
					dischargeOfRenderingNumber = ReturnDischargeOfNumber(score, 100) / 100;
					rect.x = rect.x + rect.w;
					rect.w = srcRects[10 + dischargeOfRenderingNumber].w * (pixOnBlock / 7);
					SDL_RenderCopy(renderer, textures[ALPHABET_TEXTURE], &srcRects[10 + dischargeOfRenderingNumber], &rect);
					/* Render tens */
					dischargeOfRenderingNumber = ReturnDischargeOfNumber(score, 10) / 10;
					rect.x = rect.x + rect.w;
					rect.w = srcRects[10 + dischargeOfRenderingNumber].w * (pixOnBlock / 7);
					SDL_RenderCopy(renderer, textures[ALPHABET_TEXTURE], &srcRects[10 + dischargeOfRenderingNumber], &rect);
					/* Render ones */
					dischargeOfRenderingNumber = ReturnDischargeOfNumber(score, 1);
					rect.x = rect.x + rect.w;
					rect.w = srcRects[10 + dischargeOfRenderingNumber].w * (pixOnBlock / 7);
					SDL_RenderCopy(renderer, textures[ALPHABET_TEXTURE], &srcRects[10 + dischargeOfRenderingNumber], &rect);
				}
				else {
					/* Render score from 1000 to 9999 */
					if (score < 10000) {
						/* Render thousands */
						dischargeOfRenderingNumber = ReturnDischargeOfNumber(score, 1000) / 1000;
						rect.x = rect.x + rect.w;
						rect.w = srcRects[10 + dischargeOfRenderingNumber].w * (pixOnBlock / 7);
						SDL_RenderCopy(renderer, textures[ALPHABET_TEXTURE], &srcRects[10 + dischargeOfRenderingNumber], &rect);
						/* Render hundreds */
						dischargeOfRenderingNumber = ReturnDischargeOfNumber(score, 100) / 100;
						rect.x = rect.x + rect.w;
						rect.w = srcRects[10 + dischargeOfRenderingNumber].w * (pixOnBlock / 7);
						SDL_RenderCopy(renderer, textures[ALPHABET_TEXTURE], &srcRects[10 + dischargeOfRenderingNumber], &rect);
						/* Render tens */
						dischargeOfRenderingNumber = ReturnDischargeOfNumber(score, 10) / 10;
						rect.x = rect.x + rect.w;
						rect.w = srcRects[10 + dischargeOfRenderingNumber].w * (pixOnBlock / 7);
						SDL_RenderCopy(renderer, textures[ALPHABET_TEXTURE], &srcRects[10 + dischargeOfRenderingNumber], &rect);
						/* Render ones */
						dischargeOfRenderingNumber = ReturnDischargeOfNumber(score, 1);
						rect.x = rect.x + rect.w;
						rect.w = srcRects[10 + dischargeOfRenderingNumber].w * (pixOnBlock / 7);
						SDL_RenderCopy(renderer, textures[ALPHABET_TEXTURE], &srcRects[10 + dischargeOfRenderingNumber], &rect);
					}
					/* If score > 10000 log error to console */
					else
						std::cout << "ERROR!!! Wrong score!(too big or too small)" << std::endl;
				}
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

	int RenderWithCoustomSettings(int textureFromTexturesArray, int srcRectFromSrcRectsArray, SDL_Rect rect)
	{
		if (textureFromTexturesArray >= sizeOftexturesArray)
			return -1;
		if (textureFromTexturesArray >= sizeOfsourceRectsArray)
			return -2;

		if(srcRectFromSrcRectsArray == NULL)
			SDL_RenderCopy(renderer, textures[textureFromTexturesArray], NULL, &rect);

		SDL_RenderCopy(renderer, textures[textureFromTexturesArray], &srcRects[srcRectFromSrcRectsArray], &rect);
		return 0;
	}

	void RenderCustomTexture(SDL_Texture* tex, SDL_Rect srcrect, SDL_Rect rect)
	{
		SDL_RenderCopy(renderer, tex, &srcrect, &rect);
	}

	void RenderClear()
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

	SDL_Rect GetSrcRect(int i)
	{
		if (i >= sizeOfsourceRectsArray || i < 0)
		{
			rect.x = -1;
			rect.y = -1;
			rect.w = -1;
			rect.h = -1;
			return rect;
		}

		return srcRects[i];
	}

	int GetWinW()
	{
		return winW;
	}
	int GetWinH()
	{
		return winH;
	}
};