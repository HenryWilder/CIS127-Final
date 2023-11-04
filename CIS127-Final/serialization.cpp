#include "serialization.hpp"
#include "utilities.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include <fstream>

constexpr const char filename[] = "savegame.txt";

void LoadFromFile(ifstream& ifs)
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
}

void InitNewGame()
{
    player.Init();
    surroundings.ReRoll();
}

void LoadWithPrompt()
{
    ifstream file(filename);
    if (file.is_open())
    {
        string pick = Prompt("Start", { "load", "new" });
        
        if (pick == "new")
        {
            // causes ifs.is_open() to fail
            file.close();
        }
    }
    else
    {
        cout << "No save found. Starting a new game.\n\n";
    }
    
    if (file.is_open()) // load game
    {
        LoadFromFile(file);
        
        file.close();
        
        cout << "Welcome back, " << player.GetName();
    }
    else // new game
    {
        InitNewGame();
    }
}

void SaveToFile(ofstream& ofs)
{
    player.Save(ofs);
    surroundings.Save(ofs);

    ofs << player.name << '\n'
        << player.health.health << ' '
        << player.luck.luck << ' '
        << player.inventory.items.size() << ' '
        << player.influences.influence.size() << ' '
        << surroundings.things.size();
    
    for (const auto& it : player.influences.influence)
    {
        ofs << '\n' << it.first << ' ' << it.second;
    }
    
    for (const auto& item : player.inventory.items)
    {
        ofs << '\n' << item.first << ' ' << item.second;
    }
    
    for (const auto& it : surroundings.things)
    {
        ofs << '\n' << it.first;
    }
}

void Save()
{
    ofstream file(filename, ofstream::trunc);
    if (file.is_open())
    {
        SaveToFile(file);
        file.close();
    }
}
