#include "MainMenu.h"
#include "Serialize.h"

using enum MainMenuToken;

constexpr MenuFlavor mainMenuFlavor = {};

MainMenu firstTimeMenu = {
    .flavor = mainMenuFlavor,
    .options = {
        { "new",  "New game",  NEW_GAME  },
        { "opt",  "Options",   OPTIONS   },
        { "exit", "Exit game", EXIT_GAME },
    },
};

MainMenu regularMainMenu = {
    .flavor = mainMenuFlavor,
    .options = {
        { "cont", "Continue",  CONTINUE  },
        { "opt",  "Options",   OPTIONS   },
        { "new",  "New game",  NEW_GAME  },
        { "load", "Load game", LOAD_GAME },
        { "exit", "Exit game", EXIT_GAME },
    },
};

const MainMenu& GetMainMenu()
{
    bool firstTime = !config.SaveExists();
    return firstTime ? firstTimeMenu : regularMainMenu;
}
