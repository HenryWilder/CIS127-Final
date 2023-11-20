#include "utilities.hpp"
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
    if (player.CheckInfluence(GetCollective()))
    {
        echo << "The baker thanks you for the bread and gives you 1 gold in return.";
    }
    else if (player.TryRemoveItemQty(Item::Gold, 2) != -1)
    {
        echo << "The baker thanks you for the bread and sells it back to you for 2 gold.";
    }
    else
    {
        echo << "The baker accepts the bread.";
    }
}
void Baker::DoInteraction_Sword()
{
    switch (player.TestLuck())
    {
    case LuckType::Good:
        player.AddItem(Item::Bread, 13);
        echo << "The baker crumbles into a baker's dozen loafs of bread that scatter across the floor. You rapidly snatch up each one.";
        RemoveFromWorld();
        break;

    case LuckType::Neutral:
        echo << "The baker didn't much enjoy that, but they're gone now.";
        RemoveFromWorld();
        break;

    case LuckType::Bad:
        echo << "You have angered the bread gods, and "; // todo
        break;
    }
}
void Baker::DoInteraction_Gold()
{
    player.AddItem(Item::Bread);
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
