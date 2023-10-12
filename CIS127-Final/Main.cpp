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
#include "Menu.h"
using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::getline;

enum class MainMenuToken
{
    EXIT_GAME,
    OPTIONS,
    NEW_GAME,
    CONTINUE,
    LOAD_GAME,
};

using MainMenu = Menu<MainMenuToken>;

MainMenuToken GetMainMenuSelection()
{
    using enum MainMenuToken;

    MainMenu menu;

    if (config.SaveExists())
    {
        menu.options = {
            { "cont", "Continue",  CONTINUE  },
            { "opt",  "Options",   OPTIONS   },
            { "new",  "New game",  NEW_GAME  },
            { "load", "Load game", LOAD_GAME },
            { "exit", "Exit game", EXIT_GAME },
        };
    }
    else
    {
        menu.options = {
            { "new",  "New game",  NEW_GAME  },
            { "opt",  "Options",   OPTIONS   },
            { "exit", "Exit game", EXIT_GAME },
        };
    }

    return menu.GetSelection(MenuFlavor{});
}

int main()
{
    LoadConfigData();

    bool shouldGameClose = false;

    while (!shouldGameClose)
    {
        MainMenuToken selectedMenuItem = GetMainMenuSelection();
        using enum MainMenuToken;
        switch (selectedMenuItem)
        {
        case EXIT_GAME:
            shouldGameClose = true;
            break;

        case OPTIONS:
            break;

        case NEW_GAME:
            break;

        case CONTINUE:
            break;

        case LOAD_GAME:
            break;
        }
        cout << endl;
    }

    return 0;
}
