#include "Baker.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include "TurnEchoStream.hpp"

void Baker::DoInteraction_Grab()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Bread()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Sword()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Gold()
{
    player.inventory.Add(Item::Bread);
    echo << "The baker thanks you for the gold and hands you a piece of bread.";
}

// Baker

void Baker::DoInteraction_Talk_Generic()
{
    echo << "[todo]";
}

// Potion

void Baker::DoInteraction_Potion_Predict()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Potion_Heal()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Potion_Water()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Potion_Wish()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Potion_Ducks()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Potion_Force()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Potion_Salt()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Potion_Ants()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Potion_Demon()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Potion_Fire()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Potion_Explode()
{
    echo << "[todo]";
}
void Baker::DoInteraction_Potion_Tree()
{
    echo << "[todo]";
}
