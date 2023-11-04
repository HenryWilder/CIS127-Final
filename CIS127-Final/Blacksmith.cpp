#include "Blacksmith.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include <iostream>
using namespace std;

void Blacksmith::DoInteraction_Grab()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Bread()
{
    if (player.influences.Check(collective.ShortName()))
    {
        string returnGift = ChooseRandom({ "gold", "sword" });
        int quantity = returnGift == "sword" ? 10 : 1;
        player.inventory.Add(returnGift, quantity);
        cout << "The blacksmith accepts the bread, gifting you " << quantity << " " << returnGift << " as thanks";
    }
    else
    {
        cout << "The blacksmith believes your bread is far too stale, proving their point by sharpening it into a sword and using it to slice another sword.";
    }
}
void Blacksmith::DoInteraction_Sword()
{
    player.influences.Modify(collective.ShortName(), -1);
    player.inventory.RemoveAll("sword");
    cout << "Without hesitation, the blacksmith meets your slash with a parry from their own sword.\n"
        "Your sword flies out of your hand and shatters against the wall, leaving you unarmed.";
}
void Blacksmith::DoInteraction_Gold()
{
    player.inventory.Add("sword", 10);
    cout << "The blacksmith thanks you for the gold and adds some durability to your sword.";
}

// Talk

void Blacksmith::DoInteraction_Talk_Generic()
{
    cout << "[todo]";
}

// Potion

void Blacksmith::DoInteraction_Potion_Predict()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Heal()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Water()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Wish()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Ducks()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Force()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Salt()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Ants()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Demon()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Fire()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Explode()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Potion_Tree()
{
    cout << "[todo]";
}

string Blacksmith::GetName() const
{
    return "smith";
}
