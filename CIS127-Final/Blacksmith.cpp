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
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Sword()
{
    cout << "[todo]";
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
void Blacksmith::DoInteraction_Talk_WineFish()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_SkeleStock()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_WoodpeckerSiege()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_NecroFarm()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_Bloodmoon()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_MoleMountain()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_MindflayerRecipe()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_GodFistfight()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_Theseus()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_SmithTeleken()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_NoGarlic()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_PenguinBattle()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_PetMentalHealth()
{
    cout << "[todo]";
}
void Blacksmith::DoInteraction_Talk_Woodchuck()
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
