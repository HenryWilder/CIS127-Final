#include "serialization.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include <fstream>

constexpr const char filename[] = "savegame.txt";

void Load()
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
        cout << "No save found. Starting a new game.\n\n";
    }
    
    if (ifs.is_open()) // load game
    {
        size_t numItems, numInfluences, numSurroundings;
        getline(ifs, player.name)
            >> player.health.health
            >> player.luck.luck
            >> numItems
            >> numInfluences
            >> numSurroundings;
        
        player.influences.ClearAll();
        for (size_t i = 0; i < numInfluences; ++i)
        {
            string collective;
            int influence;
            ifs >> collective >> influence;
            player.influences.Set(collective, influence);
        }
        
        player.inventory.RemoveAll();
        for (size_t i = 0; i < numItems; ++i)
        {
            string itemName;
            int itemCount;
            ifs >> itemName >> itemCount;
            player.inventory.Add(itemName, itemCount);
        }
        
        surroundings.Clear();
        for (size_t i = 0; i < numSurroundings; ++i)
        {
            string thing;
            ifs >> thing;
            surroundings.TryAddNew(thing);
        }
        
        ifs.close();
        
        cout << "Welcome back, " << player.GetName();
    }
    else // new game
    {
        cout << "What is your name?";
        player.name = "";
        while (player.name == "")
        {
            cout << "\n> ";
            getline(cin, player.name);
        }
        player.health.Set(5, 5);
        player.luck.Set(0);
        player.influences.ClearAll();
        player.inventory.RemoveAll();
        player.inventory.Add("gold", 5);
        player.inventory.Add("sword", 20);
        surroundings.ReRoll();
    }
}

void Save()
{
    ofstream ofs(filename, ofstream::trunc);
    ofs << player.name << '\n'
        << player.health.health << ' '
        << player.luck.luck << ' '
        << player.inventory.items.size() << ' '
        << player.influences.influence.size() << ' '
        << surroundings.things.size();
    
    for (const auto& it : player.influences.influence)
    {
        ofs << it.first << it.second;
    }
    
    for (const auto& item : player.inventory.items)
    {
        ofs << item.first << item.second;
    }
    
    for (const auto& it : surroundings.things)
    {
        ofs << it.first;
    }
    
    ofs.close();
}
