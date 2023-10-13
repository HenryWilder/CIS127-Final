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
#include "Perlin2D.h"
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
    TextureGrayscale test(32, 32);
    test.ApplyFragmentShader(shader_presets::clearBackground<0.1f>);
    test.ApplyFragmentShader(shader_presets::Circle(vec2(0.5f), 0.25f, 1.0f));
    test.ApplyFragmentShader(shader_presets::Circle(vec2(0.25f, 0.75f), 0.125f, 0.75f));
    test.ApplyFragmentShader(shader_presets::Rectangle(0.25f, 0.75f, 0.25f, 0.125f, 0.35f));
    test.Print();

    TextureGrayscale dest(32, 32);
    quad iso = quad{
        vec2(0.0f, 0.5f),
        vec2(0.5f, 0.25f),
        vec2(1.0f, 0.5f),
        vec2(0.5f, 0.75f)
    };
    dest.Draw(test, iso);
    dest.Print();

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
    data.map.Generate(1);
    PlayGame(data);
}

void PlayGame(SaveData& data)
{
    data.map.GetRoom(0).PrintRoom();
}
