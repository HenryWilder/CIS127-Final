/**
* CIS 127 Final | Minimum Viable Product (in case I get too in-depth with the full version)
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

#include "Utilities.h"
#include "Inventory.h"
#include "Character.h"
#include "Map.h"
#include "Serialize.h"

int main()
{
    if (!data.Load()) // New game
    {
        cout << Colored<GOLD>("STARTING NEW GAME\n");

        data = SaveData();
        data.player = Player(PromptString("What is your name?"), IVec2(0, 0));
        data.map = Map(PromptString("Map seed"));

        data.Save(); // Create save file
    }

    cout
        << "Your name is " << data.player.GetDisplayName() << ".\n"
        << "Map seed: " << data.map.GetSeed() << "\n"
        << '\n'
        << "Input \"" << Colored<GOLD>("save") << "\" at any time to save the game " << Colored<WHITE>("without closing") << ".\n"
        << "Input \"" << Colored<GOLD>("quit") << "\" at any time to save and close the game.\n"
        << "Closing the window without inputting \"" << Colored<GOLD>("quit") << "\" will NOT save your progress.\n"
        << "This game auto-saves occasionally. \"" << Colored<SKYBLUE>("[Saving...]") << "\" will be shown when this happens.\n"
        << '\n';

#if 1
    data.map._PrintDebug(data.player.GetPosition());
#endif

    data.map.GetTile(data.player.GetPosition());
    while (true)
    {
        data.map.DoMovement(data.player);
    }

    return 0;
}
