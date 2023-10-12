/**
 * CIS 127 Final
 * @author Henry Lachman
 * @description
 * Option 2 - Dungeon Crawl Text Game
 * - Store game information to a file
 *   - File must contain at least 6 pieces of information
 * - Prompt the user for directions
 * - Gather and store items
 * - Remove items from inventory when used
 * - Play must encounter other "characters"
 */
#include <iostream>
#include <string>
#include <vector>
#include "Serialize.h"
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::vector;

enum class MenuItem
{
    EXIT_GAME,
    OPTIONS,
    NEW_GAME,
    CONTINUE,
    LOAD_GAME,
};

MenuItem GetMainMenuSelection()
{
    const string MENU_ITEM_CONTINUE = "Continue";
    const string MENU_ITEM_NEWGAME = "New game";
    const string MENU_ITEM_OPTIONS = "Options";
    const string MENU_ITEM_LOAD = "Load game";
    const string MENU_ITEM_EXIT = "Exit game";
    vector<string> menuOptions;
    menuOptions.reserve(5);

    if (config.SaveExists())
    {
        menuOptions = { MENU_ITEM_CONTINUE, MENU_ITEM_OPTIONS, MENU_ITEM_LOAD, MENU_ITEM_NEWGAME, MENU_ITEM_EXIT };
    }
    else
    {
        menuOptions = { MENU_ITEM_NEWGAME, MENU_ITEM_OPTIONS, MENU_ITEM_EXIT };
    }

    for (size_t i = 0; i < menuOptions.size(); ++i)
    {
        cout << (i + 1) << ": " << menuOptions[i] << '\n';
    }

    char menuItemSelectionNumber = '0';
    while (menuItemSelectionNumber < '1' || '0' + menuOptions.size() < menuItemSelectionNumber)
    {
        cout << "Select a menu item: ";
        cin >> menuItemSelectionNumber;
    }
    const string& selectedMenuItem = menuOptions[menuItemSelectionNumber - (size_t)'1'];
    cout << "You selected " << selectedMenuItem << endl;

    if (selectedMenuItem == MENU_ITEM_CONTINUE)
    {

    }
    else if (selectedMenuItem == MENU_ITEM_EXIT)
    {
        return MenuItem::EXIT_GAME;
    }
}

int main()
{
    LoadConfigData();

    bool shouldGameClose = false;

    while (!shouldGameClose)
    {
        MenuItem selectedMenuItem = GetMainMenuSelection();
        switch (selectedMenuItem)
        {
        case MenuItem::EXIT_GAME:
            shouldGameClose = true;
            break;

        case MenuItem::OPTIONS:
            break;

        case MenuItem::NEW_GAME:
            break;

        case MenuItem::CONTINUE:
            break;

        case MenuItem::LOAD_GAME:
            break;
        }
    }

    return 0;
}
