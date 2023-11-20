#include "utilities.hpp"
#include "Door.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include "TurnEchoStream.hpp"

void Door::DoInteraction_Grab()
{
    surroundings.ReRoll();
    echo << "The door opens, allowing you through.";
}
void Door::DoInteraction_Bread()
{
    player.GiveLuck(LuckType::Good);
    echo << "The door says nothing, but you get the odd sensation that it appriciates the gift.";
}
void Door::DoInteraction_Sword()
{
    if (DisadvantagedCoinFlip())
    {
        surroundings.ReRoll();
        echo << "The door crumbles to bits and you walk through it.";
    }
    else
    {
        echo << "The door does not yield.";
    }
}
void Door::DoInteraction_Gold()
{
    echo << "The door remains silent and unmoved. It has no concept for money.";
}

// Talk

void Door::DoInteraction_Talk_Generic()
{
    echo << "The door listens silently.";
}

// Potion

void Door::DoInteraction_Potion_Predict()
{
    echo << "[todo]";
}
void Door::DoInteraction_Potion_Heal()
{
    echo << "[todo]";
}
void Door::DoInteraction_Potion_Water()
{
    echo << "[todo]";
}
void Door::DoInteraction_Potion_Wish()
{
    echo << "[todo]";
}
void Door::DoInteraction_Potion_Ducks()
{
    echo << "[todo]";
}
void Door::DoInteraction_Potion_Force()
{
    echo << "[todo]";
}
void Door::DoInteraction_Potion_Salt()
{
    echo << "[todo]";
}
void Door::DoInteraction_Potion_Ants()
{
    echo << "[todo]";
}
void Door::DoInteraction_Potion_Demon()
{
    echo << "[todo]";
}
void Door::DoInteraction_Potion_Fire()
{
    echo << "[todo]";
}
void Door::DoInteraction_Potion_Explode()
{
    echo << "[todo]";
}
void Door::DoInteraction_Potion_Tree()
{
    echo << "[todo]";
}
