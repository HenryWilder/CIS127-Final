#pragma once

enum class AutoLoadOrNew
{
    Ask  = 0,
    Load = 1,
    New  = 2,
};

// Asks the user whether to load or create a game, then loads or initializes the game variables
void LoadWithPrompt(AutoLoadOrNew loadOrNew, const char* name);
void Save();
