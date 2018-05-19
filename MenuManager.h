#pragma once

#include "MenuPage.h"

#define MAIN_MENU 0
#define OPTIONS 1
#define MULTIPLAYER 2

#define MAIN_START 1
#define MAIN_OPTIONS 2
#define MAIN_MULTIPLAYER 3
#define MAIN_EXIT 4
#define OPTIONS_PLUS 1
#define OPTIONS_MINUS 3
#define OPTIONS_BACK 6
#define MULTIPLAYER_BACK 4

class Menu
{
private:
	std::vector<MenuPage> pages;
	GameRenderer* gr;
	int currentpage = 0;
public:
	Menu(GameRenderer* gamerenderer)
	{
		gr = gamerenderer;

		/* One sixth part of screen */
		int x = gr->GetWinH() / 6;

		/* Default pages */

		/* Main menu */
		MenuPage page_mainmenu(gr);
		pages.push_back(page_mainmenu);
		pages[MAIN_MENU].addButtonInMiddle("MAIN MENU", x, 10, false);
		pages[MAIN_MENU].addButtonInMiddle("START", x*2, 7, true);
		pages[MAIN_MENU].addButtonInMiddle("OPTIONS", x*3, 7, true);
		pages[MAIN_MENU].addButtonInMiddle("MULTIPLAYER", x*4, 7, true);
		pages[MAIN_MENU].addButtonInMiddle("EXIT", x*5, 7, true);

		/* Options */
		MenuPage page_options(gr);
		pages.push_back(page_options);
		pages[OPTIONS].addButtonInMiddle("OPTIONS", x, 10, false);
		pages[OPTIONS].addButtonInMiddle("+", x * 2, 10, true);
		pages[OPTIONS].addButtonInMiddle("SPEED " +  std::to_string(300) + " MS PER MOVE", x * 3, 7, false);
		pages[OPTIONS].addButtonInMiddle("-", x * 4, 10, true);
		pages[OPTIONS].addButtonInMiddle("BACK", x * 5, 7, true);

		/* Multiplayer */
		MenuPage page_multiplayer(gr);
		pages.push_back(page_multiplayer);
		pages[MULTIPLAYER].addButtonInMiddle("MULTIPLAYER", x, 10, false);
		pages[MULTIPLAYER].addButtonInMiddle(" ", x * 2, 7, false);
		pages[MULTIPLAYER].addButtonInMiddle("COMING SOON", x * 3, 7, false);
		pages[MULTIPLAYER].addButtonInMiddle(" ", x * 4, 7, false);
		pages[MULTIPLAYER].addButtonInMiddle("BACK", x * 5, 7, true);

		pages[0].RenderPage();
	}

	void ChangeButtonTextAndStayInCenter(std::string buttonName, std::string newText)
	{
		pages[currentpage].ChangeButtonTextAndStayInCenter(buttonName, newText);
	}

	int CheckCollisions(POINT p)
	{
		return pages[currentpage].CheckCollisions(p);
	}
	int ChangeCurrentPage(int page)
	{
		if (page < 0)
			return -1;
		if (page >= pages.size())
			return -2;
		currentpage = page;
	}
	int GetCurrentPage()
	{
		return currentpage;
	}
	void RenderCurrentPage()
	{
		pages[currentpage].RenderPage();
	}
};