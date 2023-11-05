#include "serialization.hpp"
#include "utilities.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include <fstream>

constexpr const char filename[] = "savegame.txt";

void InitNewGame()
{
    player      .Init();
    surroundings.Init();
}

void LoadFromFile(ifstream& ifs)
{
    player      .Load(ifs);
    surroundings.Load(ifs);
}

void SaveToFile(ofstream& ofs)
{
    player      .Save(ofs);
    surroundings.Save(ofs);
}

void LoadWithPrompt()
{
    ifstream file(filename);
    
    // load game
    if (file.is_open() && Prompt("Start", { "load", "new" }) == "new")
    {
        LoadFromFile(file);
        cout << "Welcome back, " << player.GetName() << "\n\n";
    }
    // new game
    else
    {
        cout << "Starting a new game.\n\n";
        InitNewGame();
    }

    if (file.is_open())
    {
        file.close();
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
