#include "utilities.hpp"
#include "Door.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include "TurnEchoStream.hpp"

void Door::DoInteraction_Grab()
{
    echo << "The door opens, allowing you through.\n";
    surroundings.ReRoll();
}
void Door::DoInteraction_Bread()
{
    echo << "The door says nothing, but you get the odd sensation that it appriciates the gift.\n";
    player.GiveLuck(LuckType::Good);
}
void Door::DoInteraction_Sword()
{
    if (DisadvantagedCoinFlip())
    {
        echo << "The door crumbles to bits and you walk through it.\n";
        surroundings.ReRoll();
    }
    else
    {
        echo << "The door does not yield.\n";
    }
}
void Door::DoInteraction_Gold()
{
    echo << "The door remains silent and unmoved. It has no concept for money.\n";
}

// Talk

void Door::DoInteraction_Talk_Generic()
{
    echo << "The door listens silently.\n";
}

// Potion

void Door::DoInteraction_Potion_Predict()
{
    echo << "[todo]\n";
}
void Door::DoInteraction_Potion_Heal()
{
    echo << "[todo]\n";
}
void Door::DoInteraction_Potion_Water()
{
    echo << "[todo]\n";
}
void Door::DoInteraction_Potion_Wish()
{
    echo << "[todo]\n";
}
void Door::DoInteraction_Potion_Ducks()
{
    echo << "[todo]\n";
}
void Door::DoInteraction_Potion_Force()
{
    echo << "[todo]\n";
}
void Door::DoInteraction_Potion_Salt()
{
    echo << "[todo]\n";
}
void Door::DoInteraction_Potion_Ants()
{
    echo << "[todo]\n";
}
void Door::DoInteraction_Potion_Demon()
{
    echo << "[todo]\n";
}
void Door::DoInteraction_Potion_Fire()
{
    echo << "[todo]\n";
}
void Door::DoInteraction_Potion_Explode()
{
    echo << "[todo]\n";
}
void Door::DoInteraction_Potion_Tree()
{
    echo << "[todo]\n";
}
