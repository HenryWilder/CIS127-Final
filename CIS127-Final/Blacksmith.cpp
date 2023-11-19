#include "utilities.hpp"
#include "Blacksmith.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include "randomness.hpp"
#include "TurnEchoStream.hpp"

void Blacksmith::DoInteraction_Grab()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Bread()
{
    if (player.influences.Check(GetCollective()))
    {
        Item returnGift = ChooseRandom(Item::Gold, Item::Sword);
        int quantity = returnGift == "sword" ? 10 : 1;
        player.inventory.Add(returnGift, quantity);
        echo << "The blacksmith accepts the bread, gifting you " << quantity << " " << returnGift << " as thanks";
    }
    else
    {
        echo << "The blacksmith believes your bread is far too stale, proving their point by sharpening it into a sword and using it to slice another sword.";
    }
}
void Blacksmith::DoInteraction_Sword()
{
    player.influences.Modify(GetCollective(), -1);
    player.inventory.RemoveAll(Item::Sword);
    echo << "Without hesitation, the blacksmith meets your slash with a parry from their own sword.\n"
        "Your sword flies out of your hand and shatters against the wall, leaving you unarmed.";
}
void Blacksmith::DoInteraction_Gold()
{
    player.inventory.Add(Item::Sword, 10);
    echo << "The blacksmith thanks you for the gold and adds some durability to your sword.";
}

// Talk

void Blacksmith::DoInteraction_Talk_Generic()
{
    echo << "[todo]";
}

// Potion

void Blacksmith::DoInteraction_Potion_Predict()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Heal()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Water()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Wish()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Ducks()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Force()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Salt()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Ants()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Demon()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Fire()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Explode()
{
    echo << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Tree()
{
    echo << "[todo]";
}
