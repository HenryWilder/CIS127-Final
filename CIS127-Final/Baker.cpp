#include "utilities.hpp"
#include "Baker.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include "TurnEchoStream.hpp"

void Baker::DoInteraction_Grab()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Bread()
{
    if (player.CheckInfluence(GetCollective()))
    {
        echo << "The baker thanks you for the bread and gives you some gold in return.";
    }
    else if (player.CountItem(Item::Gold) >= 2)
    {
        echo << "The baker thanks you for the bread and sells it back to you.\n";
        player.TryRemoveItemQty(Item::Gold, 2);
        player.AddItem(Item::Bread);
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
        echo << "The baker crumbles into a baker's dozen loafs of bread that scatter across the floor. You rapidly snatch up each one.\n";
        player.AddItem(Item::Bread, 13);
        RemoveFromWorld();
        break;

    case LuckType::Neutral:
        echo << "The baker didn't much enjoy that, but they're gone now.\n";
        RemoveFromWorld();
        break;

    case LuckType::Bad:
        echo << "You have angered the bread gods, and [todo]\n"; // todo
        break;
    }
}
void Baker::DoInteraction_Gold()
{
    echo << "The baker thanks you for the gold and hands you some bread.\n";
    player.AddItem(Item::Bread);
}

// Baker

void Baker::DoInteraction_Talk_Generic()
{
    echo << "[todo]\n";
}

// Potion

void Baker::DoInteraction_Potion_Predict()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Potion_Heal()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Potion_Water()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Potion_Wish()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Potion_Ducks()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Potion_Force()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Potion_Salt()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Potion_Ants()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Potion_Demon()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Potion_Fire()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Potion_Explode()
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Potion_Tree()
{
    echo << "[todo]\n";
}
