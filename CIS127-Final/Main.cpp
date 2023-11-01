#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <map>
using namespace std;

#ifndef _countof
#define _countof(arr) (sizeof((arr)) / sizeof(decltype((arr)[0])))
#endif

#define TOPIC_WINEFISH      "the effects of water-wine alchemy on the local fish population"
#define TOPIC_SKELESTOCK    "the volatile stock price of enchanted skeleton armor"
#define TOPIC_WP_SIEGE      "how many woodpeckers it would take to breach the castle wall"
#define TOPIC_NECROFARM     "the ethicacy of using necromancy in farming-related fields"
#define TOPIC_BLOODMOON     "when the next blood moon may occur"
#define TOPIC_MOLE_MNT      "whether enough moles could in fact make a mountain out of their hill"
#define TOPIC_MF_RECIPE     "what recipes to use for preparing mind flayer"
#define TOPIC_GOD_FISTFIGHT "which gods could win in a fistfight against each other"
#define TOPIC_THESEUS       "why Theseus has been getting so fussy about their ship lately"
#define TOPIC_BS_TELEKEN    "what form of telekenesis would be the most effective for a blacksmith to use"
#define TOPIC_NO_GARLIC     "how the elder wizards should handle the recent garlic & holy water defecits"
#define TOPIC_PENGUIN_BTL   "whether the Old Realm needs more supplies or troops to survive their war of attrition against the Penguin Guild"
#define TOPIC_PET_MNTL_HP   "the effects of mind-altering spells on the mental health of familiars"
#define TOPIC_WOODCHUCK     "the quantity of wood throwable by a woodchuck in a hypothetical scenario that such a feat was possible for the creature"

string Prompt(const string& prompt, const vector<string>& options);
string PromptItem(const string& prompt, const map<string, int>& options);

void RerollSurroundings(vector<string>& surroundings);

class NotImplementedException :
    public exception
{
    const char* what() const noexcept override
    {
        return "feature not implemented";
    }
};

int main()
{
    const array<string, 14> topics = {
        TOPIC_WINEFISH,
        TOPIC_SKELESTOCK,
        TOPIC_WP_SIEGE,
        TOPIC_NECROFARM,
        TOPIC_BLOODMOON,
        TOPIC_MOLE_MNT,
        TOPIC_MF_RECIPE,
        TOPIC_GOD_FISTFIGHT,
        TOPIC_THESEUS,
        TOPIC_BS_TELEKEN,
        TOPIC_NO_GARLIC,
        TOPIC_PENGUIN_BTL,
        TOPIC_PET_MNTL_HP,
        TOPIC_WOODCHUCK,
    };

    const array<string, 4> itemNames = {
        "bread",  // Heals
        "sword",  // Hurts stuff - Player can only have one at a time, number is durability (number of swings) remaining. Smiths can repair it.
        "potion", // Effects are random
        "gold",   // 100% chance of getting items from NPCs
    };

    const array<string, 12> potionEffects = {
        "fire",
        "heal",
        "water",
        "ducks",
        "explode",
        "wish",
        "demon",
        "salt",
        "tree",
        "ants",
        "force",
        "predict",
    };
    
    constexpr const char filename[] =
        "savegame.txt";
        
    string playerName;
    int health;
    map<string, int> items;
    vector<string> surroundings;
    
    // Load the game
    {
        ifstream ifs(filename);
        if (ifs.is_open())
        {
            string pick = Prompt("Start", { "load", "new" });

            if (pick == "new")
            {
                // causes ifs.is_open() to fail
                ifs.close();
            }
        }
        else
        {
            cout << "No save found. Starting a new game.\n";
        }
        
        if (ifs.is_open()) // load game
        {
            size_t numItems, numSurroundings;
            getline(ifs, playerName) >> health >> numItems >> numSurroundings;
            
            for (size_t i = 0; i < numItems; ++i)
            {
                string itemName;
                int itemCount;
                ifs >> itemName >> itemCount;
                items.emplace(itemName, itemCount);
            }
            
            surroundings.reserve(numSurroundings);
            for (size_t i = 0; i < numSurroundings; ++i)
            {
                string thing;
                ifs >> thing;
                surroundings.push_back(thing);
            }
            
            ifs.close();
            
            cout << "Welcome back, " << playerName;
        }
        else // new game
        {
            cout << "What is your name?";
            playerName = "";
            while (playerName == "")
            {
                cout << "\n> ";
                getline(cin, playerName);
            }
            health = 3;
            items = { { "gold", 5 }, { "sword", 100 } };
            RerollSurroundings(surroundings);
        }
    }
    
    cout << endl;
    
    // Game loop
    while (true)
    {
        // Echo surroundings
        cout << "Your current surroundings:";
        for (const string& thing : surroundings)
        {
            cout << "\n- " << thing;
        }
        cout << endl;
        
        string cmd = Prompt("What would you like to do?", { "move", "use", "quit" });
            
        if (cmd == "move")
        {
            (void)Prompt("Where would you like to move?", { "left", "right" "forward" });
            RerollSurroundings(surroundings); // choice is an illusion :P
        }
        else if (cmd == "use")
        {
            // Request
            string action = Prompt("What would you like to use?", { "talk", "grab", "item" });
            
            if (action == "item")
            {
                if (!items.empty())
                {
                    action = PromptItem("Which item?", items);
                    --items.at(action); // Reduce item by one when used
                }
                else
                {
                    cout << "You don't have any items.\n";
                    continue;
                }
            }
            
            vector<string> availableTargets = surroundings;
            availableTargets.insert(availableTargets.begin() + 0, "self");
            string target = Prompt("On what?", availableTargets);
            
            string topic_or_effect;
            if (action == "talk")
            {
                topic_or_effect = topics.at(rand() % topics.size());
            }
            else if (action == "potion")
            {
                topic_or_effect = potionEffects.at(rand() % potionEffects.size());
            }
            
            // Echo
            cout << "You ";
            if (action == "talk")
            {
                cout << "had an interesting discussion regarding " << topic_or_effect << " with";
            }
            else if (action == "grab")
            {
                cout << "grabbed";
            }
            else if (action == "bread")
            {
                cout << "gave a piece of bread to";
            }
            else if (action == "sword")
            {
                cout << "swung your sword at";
            }
            else if (action == "potion")
            {
                cout << "used a potion of " << topic_or_effect << " on";
            }
            else if (action == "gold")
            {
                cout << "gave some gold to";
            }
            else
            {
                throw new NotImplementedException();
            }
            
            cout << " ";
            
            if (target == "self")
            {
                cout << "yourself";
            }
            else // npc
            {
                cout << "the " << target;
            }
            cout << ".\n";
            
            // Responses
            if (target == "self")
            {
                if (action == "talk")
                {
                    if (!surroundings.empty())
                    {
                        array<string, 6> possibleResponses = {
                            "found it rather odd",
                            "thinks you might need a checkup",
                            "considers your points rather thought-provoking and insightful, if a little one-sided",
                            "is too distracted by your utterly repulsive views to care that you were talking to a stone wall",
                            "is confused who you were talking to",
                            "looked around to check if there were any hidden familiars you may have been putting on a performance for",
                        };
                        // It is by design that the responder might be inanimate. I thought it would be funny.
                        string responder = surroundings.at(rand() % surroundings.size());
                        string response = possibleResponses.at(rand() % possibleResponses.size());
                        cout << "The " << responder << " " << response << ".";
                    }
                    else
                    {
                        cout << "Fortunately nobody was around to hear it.";
                    }
                }
                else if (action == "grab")
                {
                    cout << "Weirdo.";
                }
                else if (action == "bread")
                {
                    
                }
                else if (action == "potion")
                {
                    
                }
                else if (action == "gold")
                {
                    cout << "You made a show of thanking yourself kindly for the gold before returning it to your money pouch.";
                }
                else
                {
                    throw new NotImplementedException();
                }
            }
            else if (target == "door")
            {
                
            }
            else if (target == "baker")
            {
                
            }
            else if (target == "smith")
            {
                
            }
            else if (target == "wizard")
            {
                
            }
            else if (target == "enemy")
            {
                
            }
            else
            {
                throw new NotImplementedException();
            }
            
            cout << endl;
        }
        else if (cmd == "quit")
        {
            break;
        }
    }
    
    // Save the game
    {
        ofstream ofs(filename, ofstream::trunc);
        ofs << playerName << endl;
        ofs.close();
    }
    
    return 0;
}

string Prompt(const string& prompt, const vector<string>& options)
{
    cout << prompt;
    for (const string& opt : options)
    {
        cout << "\n- " << opt;
    }
    cout << endl;
    while (true) // repeats until return
    {
        string input;
        cout << "> ";
        getline(cin, input);
        auto it = find(options.begin(), options.end(), input);
        if (it != options.end())
        {
            return *it;
        }
    }
}

string PromptItem(const string& prompt, const map<string, int>& options)
{
    cout << prompt;
    for (const auto& it : options)
    {
        cout << "\n- " << it.first << " : " << it.second;
    }
    cout << endl;
    while (true) // repeats until return
    {
        string input;
        cout << "> ";
        getline(cin, input);
        auto it = options.find(input);
        if (it != options.end())
        {
            if (it->second != 0)
            {
                return it->first;
            }
            else
            {
                cout << "You don't have any " << it->first << ".\n";
            }
        }
    }
}

void RerollSurroundings(vector<string>& surroundings)
{
    surroundings.clear();
    
    vector<string> possible = {
        "door",   // Do nothing :P
        "baker",  // Give bread
        "smith",  // Repair sword
        "wizard", // Give potion
        "enemy",  // Hurt the player
    };
    
    for (int i = 0; i < 4; ++i) // Four possible items per position; often nothing
    {
        size_t index = rand() % possible.size() * 2;
        if (index < possible.size())
        {
            surroundings.push_back(possible.at(index));
            possible.erase(possible.begin() + index);
        }
    }
}
