#include "Door.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include <iostream>
using namespace std;

void Door::DoInteraction_Grab()
{
    surroundings.ReRoll();
    cout << "The door opens, allowing you through.";
}
void Door::DoInteraction_Bread()
{
    player.luck.Give(1);
    cout << "The door says nothing, but you get the odd sensation that it appriciates the gift.";
}
void Door::DoInteraction_Sword()
{
    if (DiceCheck(3, 4))
    {
        cout << "The door does not yield.";
    }
    else
    {
        surroundings.ReRoll();
        cout << "The door crumbles to bits and you walk through it.";
    }
}
void Door::DoInteraction_Gold()
{
    cout << "The door remains silent and unmoved. It has no concept for money.";
}

// Talk

void Door::DoInteraction_Talk_Generic()
{
    cout << "The door listens silently.";
}
void Door::DoInteraction_Talk_WineFish()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_SkeleStock()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_WoodpeckerSiege()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_NecroFarm()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_Bloodmoon()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_MoleMountain()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_MindflayerRecipe()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_GodFistfight()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_Theseus()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_SmithTeleken()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_NoGarlic()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_PenguinBattle()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_PetMentalHealth()
{
    cout << "[todo]";
}
void Door::DoInteraction_Talk_Woodchuck()
{
    cout << "[todo]";
}

// Potion

void Door::DoInteraction_Potion_Predict()
{
    cout << "[todo]";
}
void Door::DoInteraction_Potion_Heal()
{
    cout << "[todo]";
}
void Door::DoInteraction_Potion_Water()
{
    cout << "[todo]";
}
void Door::DoInteraction_Potion_Wish()
{
    cout << "[todo]";
}
void Door::DoInteraction_Potion_Ducks()
{
    cout << "[todo]";
}
void Door::DoInteraction_Potion_Force()
{
    cout << "[todo]";
}
void Door::DoInteraction_Potion_Salt()
{
    cout << "[todo]";
}
void Door::DoInteraction_Potion_Ants()
{
    cout << "[todo]";
}
void Door::DoInteraction_Potion_Demon()
{
    cout << "[todo]";
}
void Door::DoInteraction_Potion_Fire()
{
    cout << "[todo]";
}
void Door::DoInteraction_Potion_Explode()
{
    cout << "[todo]";
}
void Door::DoInteraction_Potion_Tree()
{
    cout << "[todo]";
}

string Door::GetName() const
{
    return "door";
}
