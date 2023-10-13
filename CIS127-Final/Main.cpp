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
#include "MainMenu.h"
#include "LevelMap.h"
#include "RNG.h"
using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::getline;

void NewGame();
void PlayGame(SaveData& data);

int main()
{
    srand(time(nullptr));
    cout << Roll(8, D6) + Roll(2, D8, +5) << endl;
    cout << Roll(8, D6) + Roll(2, D8, +5) << endl;
    cout << Roll(8, D6) + Roll(2, D8, +5) << endl;
    cout << Roll(8, D6) + Roll(2, D8, +5) << endl;
    cout << Roll(8, D6) + Roll(2, D8, +5) << endl;
    return 0;

    LoadConfigData();

    bool shouldGameClose = false;
    while (!shouldGameClose)
    {
        MainMenuToken selectedMenuItem = GetMainMenu().GetSelection();
        cout << endl;

        using enum MainMenuToken;

        switch (selectedMenuItem)
        {
        case EXIT_GAME: shouldGameClose = true; break;
        case OPTIONS: break;
        case NEW_GAME: NewGame(); break;
        case CONTINUE: break;
        case LOAD_GAME: break;
        }
    }

    return 0;
}

void NewGame()
{
    LevelMap map;
    map.Generate(3);
    SaveData data;
    PlayGame(data);
}

void PlayGame(SaveData& data)
{
    data.map.PrintMap();
}
