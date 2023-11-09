#pragma once
#include "utilities.hpp"
#include "TurnEchoStream.hpp"
#include "randomness.hpp"
#include "Prompt.hpp"
#include "Enums.hpp"
#include "Interactable.hpp"
#include "Baker.hpp"
#include "Blacksmith.hpp"
#include "Wizard.hpp"
#include "Monster.hpp"
#include "Door.hpp"
#include "Components.hpp"
#include "Player.hpp"
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
    static bool IsPlayerSoftLocked()
    {
        return player.health.statuses.Has(StatusEffects::Tree);
    }

    static bool IsPlayerWithoutItems()
    {
        return player.inventory.IsEmpty();
    }

public:
    TurnEndType GetTurnEndType() const
    {
        return turnEndType;
    }

    static Action PromptForAction();
    static pair<EntityTypeInfo_t, Entity&> PromptForTartget(const string& prompt);

    static void DoMove();
    static void DoTalk();
    static void DoGrab();
    static void DoUseItem(Item item, EntityTypeInfo_t target, Entity& targetObject);
    static void DoUse();
    static void DoItems();
    static void DoNear();
    void DoQuit();
    void DoRestart();
    void DoAction(Action action);

    // Returns false if dead
    bool CheckPlayerIsAlive();

private:
    TurnEndType turnEndType = TurnEndType::Continue;
};

extern TurnHandler turnHandler;
