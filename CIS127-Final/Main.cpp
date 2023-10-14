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
    DrawPixel({ 255, 0, 255 });
    DrawPixel({ 32, 32, 32 });
    std::cout << '\n';
    DrawPixel({ 32, 32, 32 });
    DrawPixel({ 255, 0, 255 });
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
    {
        TextureGrayscale roomTex(16, 16);
        data.map.GetRoom(0).GetRoomTexture(roomTex);
        roomTex.Print();
        roomTex.PrintIso(2.0f);
    }
    {
        auto [width, height] = data.map.GetMapSize();
        TextureGrayscale mapTex(width, height);
        data.map.GetMapTexture(mapTex);
        mapTex.Print();
        //mapTex.PrintIso(2.0f);
    }
}
