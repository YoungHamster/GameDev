#pragma once
#include <string>
#include <Windows.h>
#include "GamePhysics.h"
#include "GameRender.h"

/* Letter and number codes defined in GameRender.h */
int CharToLetterCode(char c)
{
	/* Number */
	if (c < 58 && c > 47)
	{
		return (int)c - 38;
	}
	/* Capital letter */
	if (c < 91 && c > 64)
	{
		return (int)c - 44;
	}
	/* Small letter */
	if (c < 123 && c > 96)
	{
		return (int)c - 76;
	}
	if (c == 32)
		return SPACE;
	if (c == 47)
		return SLASH;
	if (c == 43)
		return PLUS;
	if (c == 45)
		return MINUS;
	return -1;
}

class Button
{
private:
	int width;
	int heigth;
	int fontSize;
	AABB ClickBox;
	std::string text;
	bool clickAble;
	bool enlarged = false;
	GameRenderer* gr;
	std::vector<int> lettersCodes;
	std::vector<SDL_Rect> lettersRects;
public:
	/* Constructor, fills button with all parameters of it */
	Button(std::string button_text, int x, int y, int FontSize, bool clickable, GameRenderer* gamerenderer)
	{
		fontSize = FontSize;
		text = button_text;
		clickAble = clickable;
		gr = gamerenderer;

		ClickBox.min.x = x;
		ClickBox.min.y = y;

		lettersCodes.resize(text.length());
		lettersRects.resize(lettersCodes.size());
		for (int i = 0; i < text.length(); i++)
		{
			lettersCodes[i] = CharToLetterCode(text[i]);
		}
		int offset = 0;
		for (int i = 0; i < lettersRects.size(); i++)
		{
			lettersRects[i].x = x + offset;
			lettersRects[i].y = y;
			lettersRects[i].w = gr->GetSrcRect(lettersCodes[i]).w * FontSize;
			lettersRects[i].h = gr->GetSrcRect(lettersCodes[i]).h * FontSize;
			offset = offset + lettersRects[i].w;
		}
		ClickBox.max.x = lettersRects[lettersRects.size() - 1].x + lettersRects[lettersRects.size() - 1].w;
		ClickBox.max.y = lettersRects[lettersRects.size() - 1].y + lettersRects[lettersRects.size() - 1].h;

		width = ClickBox.max.x - ClickBox.min.x;
		heigth = ClickBox.max.y - ClickBox.min.y;
	}

	/* Renders button in buffer */
	void RenderButton()
	{
		int previousFontSize;
		if (enlarged == true) {
			previousFontSize = fontSize;
			ResizeAndStayInCenter(fontSize + fontSize / 4);
		}
		for (int i = 0; i < lettersRects.size(); i++)
		{
			gr->RenderWithCoustomSettings(ALPHABET_TEXTURE, lettersCodes[i], lettersRects[i]);
		}
		if (enlarged == true) {
			fontSize = previousFontSize;
			ResizeAndStayInCenter(fontSize);
		}
	}

	bool CheckCollision(POINT cursor)
	{
		AABB cursorBox;
		cursorBox.max.x = cursor.x;
		cursorBox.max.y = cursor.y;
		cursorBox.min.x = cursor.x - 1;
		cursorBox.min.y = cursor.y - 1;
		if (AABBvsAABB(ClickBox, cursorBox))
		{
			if (clickAble)
			{
				enlarged = true;
				return true;
			}
		}
		else
			enlarged = false;
		return false;
	}

	/* Move button to x and y */
	void MoveButtonToXY(int x, int y)
	{
		/* Calculate offsets */
		int Xoffset = x - lettersRects[0].x;
		int Yoffset = y - lettersRects[0].y;
		for (int i = 0; i < lettersRects.size(); i++)
		{
			lettersRects[i].x = lettersRects[i].x + Xoffset;
			lettersRects[i].y = lettersRects[i].y + Yoffset;
		}
		/* Update Click box */
		ClickBox.min.x = lettersRects[0].x;
		ClickBox.min.y = lettersRects[0].y;
		ClickBox.max.x = lettersRects[lettersRects.size() - 1].x + lettersRects[lettersRects.size() - 1].w;
		ClickBox.max.y = lettersRects[lettersRects.size() - 1].y + lettersRects[lettersRects.size() - 1].h;
		/* Update button width and heigth*/
		width = ClickBox.max.x - ClickBox.min.x;
		heigth = ClickBox.max.y - ClickBox.min.y;
	}

	/* Cetralizes button on x axis*/
	void CentralizeButton()
	{

		MoveButtonToXY(gr->GetWinW() / 2 - width / 2, ClickBox.min.y);
		/* Update clickbox */
		ClickBox.min.x = lettersRects[0].x;
		ClickBox.min.y = lettersRects[0].y;
		ClickBox.max.x = lettersRects[lettersRects.size() - 1].x + lettersRects[lettersRects.size() - 1].w;
		ClickBox.max.y = lettersRects[lettersRects.size() - 1].y + lettersRects[lettersRects.size() - 1].h;
	}

	/* Moves button on X and Y offsets */
	void MoveButtonByOffset(int Xoffset, int Yoffset)
	{
		/* Move button */
		for (int i = 0; i < lettersRects.size(); i++)
		{
			lettersRects[i].x = lettersRects[i].x + Xoffset;
			lettersRects[i].y = lettersRects[i].y + Yoffset;
		}
		/* Update clickbox */
		ClickBox.min.x = lettersRects[0].x;
		ClickBox.min.y = lettersRects[0].y;
		ClickBox.max.x = lettersRects[lettersRects.size() - 1].x + lettersRects[lettersRects.size() - 1].w;
		ClickBox.max.y = lettersRects[lettersRects.size() - 1].y + lettersRects[lettersRects.size() - 1].h;
		/* Update width and heigth */
		width = ClickBox.max.x - ClickBox.min.x;
		heigth = ClickBox.max.y - ClickBox.min.y;
	}

	/* Resize's button and centralize's it */
	void ResizeAndStayInCenter(int NewFontSize)
	{
		fontSize = NewFontSize;
		/* X axis offset from start of string to current handling letter */
		int offset = 0;
		/* Resize all rects */
		for (int i = 0; i < lettersRects.size(); i++)
		{
			lettersRects[i].x = ClickBox.min.x + offset;
			lettersRects[i].w = gr->GetSrcRect(lettersCodes[i]).w * NewFontSize;
			lettersRects[i].h = gr->GetSrcRect(lettersCodes[i]).h * NewFontSize;
			offset = offset + lettersRects[i].w;
		}
		/* Update Click box */
		ClickBox.max.x = lettersRects[lettersRects.size() - 1].x + lettersRects[lettersRects.size() - 1].w;
		ClickBox.max.y = lettersRects[lettersRects.size() - 1].y + lettersRects[lettersRects.size() - 1].h;
		/* Update width and heigth*/
		width = ClickBox.max.x - ClickBox.min.x;
		heigth = ClickBox.max.y - ClickBox.min.y;
		/* Centralize button */
		CentralizeButton();
		/* Update Click box */
		ClickBox.min.x = lettersRects[0].x;
		ClickBox.min.y = lettersRects[0].y;
		ClickBox.max.x = lettersRects[lettersRects.size() - 1].x + lettersRects[lettersRects.size() - 1].w;
		ClickBox.max.y = lettersRects[lettersRects.size() - 1].y + lettersRects[lettersRects.size() - 1].h;
	}

	/* Changes button's text */
	void ChangeText(std::string newText)
	{
		text = newText;
		lettersCodes.resize(text.length());
		lettersRects.resize(lettersCodes.size());

		for (int i = 0; i < text.length(); i++)
		{
			lettersCodes[i] = CharToLetterCode(text[i]);
		}
		int offset = 0;
		for (int i = 0; i < lettersRects.size(); i++)
		{
			lettersRects[i].x = lettersRects[0].x + offset;
			lettersRects[i].y = lettersRects[0].y;
			lettersRects[i].w = gr->GetSrcRect(lettersCodes[i]).w * fontSize;
			lettersRects[i].h = gr->GetSrcRect(lettersCodes[i]).h * fontSize;
			offset = offset + lettersRects[i].w;
		}
		ClickBox.max.x = lettersRects[lettersRects.size() - 1].x + lettersRects[lettersRects.size() - 1].w;
		ClickBox.max.y = lettersRects[lettersRects.size() - 1].y + lettersRects[lettersRects.size() - 1].h;

		width = ClickBox.max.x - ClickBox.min.x;
		heigth = ClickBox.max.y - ClickBox.min.y;
	}

	/* Changes button's text and keep button in center */
	void ChangeTextAndStayInCenter(std::string newText)
	{
		ChangeText(newText);
		CentralizeButton();
	}

	/* Return text in string format */
	std::string getText()
	{
		return text;
	}
	/* Return button width */
	int getWidth()
	{
		return width;
	}
	/* Return button height */
	int getHeigth()
	{
		return heigth;
	}
	/* Return clickbox */
	AABB getClickBox()
	{
		return ClickBox;
	}
};