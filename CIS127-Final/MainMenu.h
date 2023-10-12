#pragma once
#include "Menu.h"

enum class MainMenuToken
{
    EXIT_GAME,
    OPTIONS,
    NEW_GAME,
    CONTINUE,
    LOAD_GAME,
};

using MainMenu = Menu<MainMenuToken>;
const MainMenu& GetMainMenu();
