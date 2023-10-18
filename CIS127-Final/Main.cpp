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
#include <HenryLib.h>
#include <ConsoleGraphics.h>
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
    DrawColoredText("DEBUG TESTS\n", Color(255, 0, 255));


    Image test;
    test.LoadFromBitmap("test.bmp");
    std::cout << "Basic bitmap rendering\n";
    test.Print();
    test.Unload();

    std::cout << std::endl;

    Image background0;
    background0.LoadFromBitmap("background0.bmp");
    std::cout << "Basic bitmap rendering\n";
    background0.Print();
    std::cout << "3x upscaling with nearest neighbor filtering\n";
    background0.Print(3.0f, { .filtering = FilterMethod::NEAREST_NEIGHBOR });
    std::cout << "3x upscaling with bilinear filtering\n";
    background0.Print(3.0f, { .filtering = FilterMethod::BILINEAR });
    std::cout << "1/2x scale with bilinear filtering and wrapping\n";
    background0.PrintEx(rect(0, 0, 16, 16), irect(0, 0, 64, 8), vec2(0.125f, 1.0f), {.xWrap = true, .filtering = FilterMethod::BILINEAR});
    background0.Unload();

    std::cout << std::endl;
    
    Image large_test;
    large_test.LoadFromBitmap("large_test.bmp");
    std::cout << "Nearest neighbor downscaling\n";
    large_test.Print(1.0f / 32, { .filtering = FilterMethod::NEAREST_NEIGHBOR });
    std::cout << "Bilinear downscaling\n";
    large_test.Print(1.0f / 32, { .filtering = FilterMethod::BILINEAR });
    std::cout << "Supersampled downscaling\n";
    large_test.Print(1.0f / 32, { .filtering = FilterMethod::PLANTERS_AVERAGE });
    large_test.Unload();

    std::cout << std::endl;

    DrawColoredText("END OF TESTS\n\n", Color(255, 0, 255));

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
