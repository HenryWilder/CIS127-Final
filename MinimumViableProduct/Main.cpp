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
#include "Serialize.h"

int main()
{
    const char* filename = "save.txt";

    if (!data.Load(filename)) // New game
    {
        cout << Colored("STARTING NEW GAME\n", GOLD);

        data = SaveData();

        data.player.name = PromptString("What is your name?");

        data.Save(filename); // Create save file
    }

    cout
        << "Your name is " << data.player.name << ".\n"
        << '\n'
        << "Input \"" << Colored("save", GOLD) << "\" at any time to save the game " << Colored("without closing", WHITE) << ".\n"
        << "Input \"" << Colored("quit", GOLD) << "\" at any time to save and close the game.\n"
        << "Closing the window without inputting \"" << Colored("quit", GOLD) << "\" will NOT save your progress.\n"
        << "This game auto-saves occasionally. \"" << Colored("[Saving...]", SKYBLUE) << "\" will be shown when this happens.\n"
        << '\n';

    data.player.inventory.Add(Item::BASIC_SWORD);

    while (true)
    {
        vector<string> options = { "items" };
        auto it = Prompt("Action", options);

        if (it == options.end())
        {
            data.Save(filename);
            return 0;
        }

        if (*it == "items")
        {
            cout << data.player.inventory << '\n';
        }

        data.player.inventory.Add(Item::DUST);
    }

    return 0;
}
