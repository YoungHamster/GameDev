#pragma once

#include "Button.h"

bool isStringsEqual(std::string string1, std::string string2)
{
	if (string1.length() != string2.length())
		return false;

	for (int i = 0; i < string1.length(); i++)
	{
		if (string1[i] != string2[i])
			return false;
	}
	return true;
}

class MenuPage
{
private:
	std::vector<Button> buttons;
	GameRenderer* gr;
public:
	MenuPage(GameRenderer* gamerenderer)
	{
		gr = gamerenderer;
	}

	/* Renders page */
	void RenderPage()
	{
		gr->RenderClear();
		gr->RenderBackground();
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i].RenderButton();
		}
		gr->RenderPresent();
	}

	/* Check colisions betwen cursor and all buttons on page */
	int CheckCollisions(POINT cursor)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i].CheckCollision(cursor))
				return i;
				//return buttons[i].getText();
		}
		return -1;
	}

	/* Add button */
	void addButton(std::string text, int x, int y, int FontSize, bool clickable)
	{
		Button button(text, x, y, FontSize, clickable, gr);
		buttons.push_back(button);
	}
	/* Add button and centralize it*/
	void addButtonInMiddle(std::string text, int y, int FontSize, bool clickable)
	{
		Button button(text, 0, y, FontSize, clickable, gr);
		int middleX = gr->GetWinW() / 2 - button.getWidth() / 2;
		button.MoveButtonToXY(middleX, y);
		buttons.push_back(button);
	}
	/* Delete button */
	void deleteButton(std::string buttonName)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (isStringsEqual(buttons[i].getText(), buttonName))
				buttons.erase(buttons.begin() + i);
		}
	}

	/* Resize button and centralize it */
	void ResizeButtonAndKeepInCenter(std::string buttonName, int FontSize)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (isStringsEqual(buttons[i].getText(), buttonName)) {
				buttons[i].ResizeAndStayInCenter(FontSize);
			}
		}
	}
	/* Move button to x y */
	void moveButton(std::string buttonName, int x, int y)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (isStringsEqual(buttons[i].getText(), buttonName)) {
				buttons[i].MoveButtonToXY(x, y);
			}
		}
	}

	/* Change button text and keep button in center */
	void ChangeButtonTextAndStayInCenter(std::string buttonName, std::string newText)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (isStringsEqual(buttons[i].getText(), buttonName))
				buttons[i].ChangeTextAndStayInCenter(newText);
		}
	}
	
	/* Get button width*/
	int getButtonWidth(std::string buttonName)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (isStringsEqual(buttons[i].getText(), buttonName))
				return buttons[i].getWidth();
		}
		return -1;
	}
	/* Get button heigth */
	int getButtonHeigth(std::string buttonName)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (isStringsEqual(buttons[i].getText(), buttonName))
				return buttons[i].getHeigth();
		}
		return -1;
	}
	/* Get click box of button */
	AABB getButtonClickBox(std::string buttonName)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (isStringsEqual(buttons[i].getText(), buttonName))
				return buttons[i].getClickBox();
		}
	}
};