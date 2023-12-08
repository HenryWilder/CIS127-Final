#include "utilities/pch/utilities.hpp"
#include "serialization/serialization.hpp"
#include "entities/player/Player.hpp"
#include "helpers/Surroundings.hpp"

constexpr const char filename[ ] = "savegame.txt";

void InitNewGame( )
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

void LoadWithPrompt(AutoLoadOrNew loadOrNew, const char* name)
{
    if (loadOrNew == AutoLoadOrNew::New)
    {
        player.SetName(name);
        InitNewGame( );
    }
    else
    {
        ifstream file(filename);
        if (loadOrNew == AutoLoadOrNew::Load)
        {
            if (file.is_open( ))
            {
                LoadFromFile(file);
                file.close();
            }
            else
            {
                cout << "Error: No save file exists.\n" << endl;
                exit(1);
            }
        }
        else
        {
            if (file.is_open( ))
            {
                cout << "Save file found" << endl;
                const string startOption = PromptOption("Load or start a new game?", { "load", "new" });
                if (startOption == string("new"))
                {
                    file.close( ); // Prevents file.is_open() from succeeding
                }
            }

            // load game
            if (file.is_open( ))
            {
                LoadFromFile(file);
                file.close( );
            }
            // new game
            else
            {
                cout << "Starting a new game.\n\n";
                player.SetName(PromptLine("What's your name?"));
                InitNewGame( );
            }
        }
    }
    cout << "Hello, " << player.GetName( ) << "\n";
}

void Save( )
{
    ofstream file(filename, ofstream::trunc);
    if (file.is_open( ))
    {
        SaveToFile(file);
        file.close( );
    }
}
