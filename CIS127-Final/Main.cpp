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
#include "Utility.h"
#include "Serialize.h"
#include "Menu.h"
#include "MainMenu.h"
#include "LevelMap.h"
#include "RNG.h"
#include "Visuals.h"
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
    Image test;
    test.LoadFromBitmap("test.bmp");
    test.Print();
    test.Unload();
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
    SaveData data;
    data.map.Generate(9);
    PlayGame(data);
}

void PlayGame(SaveData& data)
{
    cout << "Current room:\n";
    data.map.GetRoom(0).Print();
}
