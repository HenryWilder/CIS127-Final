#pragma once
#include "utilities/pch/utilities.hpp"
#include "echo.hpp"
#include "utilities/randomness.hpp"
#include "utilities/prompt.hpp"
#include "helpers/enums.hpp"
#include "entities/npcs/Baker.hpp"
#include "entities/npcs/Blacksmith.hpp"
#include "entities/npcs/Wizard.hpp"
#include "entities/npcs/Monster.hpp"
#include "entities/npcs/Door.hpp"
#include "entities/player/Player.hpp"
#include "Surroundings.hpp"

enum class TurnEndType
{
    Continue,
    Quit,
    Restart,
};

class TurnHandler
{
private:
    static bool IsPlayerSoftLocked( )
    {
        return player.HasStatusEffect(StatusEffects::Tree);
    }

    static bool IsPlayerWithoutItems( )
    {
        return player.IsInventoryEmpty( );
    }

public:
    TurnEndType GetTurnEndType( ) const
    {
        return turnEndType;
    }

    static Action PromptForAction( );
    static pair<EntityTypeInfo_t, Entity &> PromptForTartget(const string &prompt);

    static void DoMove ( );
    static void DoTalk ( );
    static void DoGrab ( );
    static void DoUseItem(Item item, EntityTypeInfo_t target, Entity &targetObject);
    static void DoUse  ( );
    static void DoItems( );
    static void DoNear ( );

    void DoQuit   ( );
    void DoRestart( );
    void DoAction (Action action);

    // Returns false if dead
    bool CheckPlayerIsAlive( );

private:
    TurnEndType turnEndType = TurnEndType::Continue;
};

extern TurnHandler turnHandler;
