#include "Baker.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include <iostream>
using namespace std;

void Baker::DoInteraction_Grab()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Bread()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Sword()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Gold()
{
    player.inventory.Add("bread");
    cout << "The baker thanks you for the gold and hands you a piece of bread.";
}

// Baker

void Baker::DoInteraction_Talk_Generic()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_WineFish()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_SkeleStock()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_WoodpeckerSiege()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_NecroFarm()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_Bloodmoon()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_MoleMountain()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_MindflayerRecipe()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_GodFistfight()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_Theseus()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_SmithTeleken()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_NoGarlic()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_PenguinBattle()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_PetMentalHealth()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Talk_Woodchuck()
{
    cout << "[todo]";
}

// Potion

void Baker::DoInteraction_Potion_Predict()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Potion_Heal()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Potion_Water()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Potion_Wish()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Potion_Ducks()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Potion_Force()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Potion_Salt()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Potion_Ants()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Potion_Demon()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Potion_Fire()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Potion_Explode()
{
    cout << "[todo]";
}
void Baker::DoInteraction_Potion_Tree()
{
    cout << "[todo]";
}

string Baker::GetName() const
{
    return "baker";
}
