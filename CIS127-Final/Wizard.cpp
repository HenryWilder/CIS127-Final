#include "Wizard.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include <iostream>
using namespace std;

void Wizard::DoInteraction_Grab()
{
    string reaction, actionDescriptor;
    if (DiceCheck(1, 2))
    {
        reaction = ChooseRandom({ "intrigued", "embarrassed" });
        actionDescriptor = ChooseRandom({ "surprised", "unamused", "annoyed", "" });
    }
    else
    {
        reaction = ChooseRandom({ "surprised", "unamused", "annoyed", "" });
        actionDescriptor = ChooseRandom({ "surprised", "unamused", "annoyed", "" });
    }
    cout << "The wizard appears " << reaction << " by your " << actionDescriptor << " action.";
}
void Wizard::DoInteraction_Bread()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Sword()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Gold()
{
    player.inventory.Add("potion");
    cout << "The wizard thanks you for the gold and gives you a potion.";
}

// Talk

void Wizard::DoInteraction_Talk_Generic()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_WineFish()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_SkeleStock()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_WoodpeckerSiege()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_NecroFarm()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_Bloodmoon()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_MoleMountain()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_MindflayerRecipe()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_GodFistfight()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_Theseus()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_SmithTeleken()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_NoGarlic()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_PenguinBattle()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_PetMentalHealth()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Talk_Woodchuck()
{
    cout << "[todo]";
}

// Potion

void Wizard::DoInteraction_Potion_Predict()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Potion_Heal()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Potion_Water()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Potion_Wish()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Potion_Ducks()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Potion_Force()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Potion_Salt()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Potion_Ants()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Potion_Demon()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Potion_Fire()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Potion_Explode()
{
    cout << "[todo]";
}
void Wizard::DoInteraction_Potion_Tree()
{
    cout << "[todo]";
}

string Wizard::GetName() const
{
    return "wizard";
}
