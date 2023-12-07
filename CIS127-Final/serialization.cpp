#include "utilities.hpp"
#include "serialization.hpp"
#include "Reader.hpp"
#include "Writer.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"

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

void LoadWithPrompt( )
{
    ifstream file(filename);

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
        cout << "Welcome back, " << player.GetName( ) << "\n";
    }
    // new game
    else
    {
        cout << "Starting a new game.\n\n";
        InitNewGame( );
        cout << "Hello, " << player.GetName( ) << "\n";
    }
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
