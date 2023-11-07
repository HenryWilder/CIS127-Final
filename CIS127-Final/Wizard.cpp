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
        reaction = ChooseRandom({ "surprised", "intrigued", "embarrassed" });
    }
    else
    {
        reaction = ChooseRandom({ "surprised", "unamused", "annoyed" });
    }
    cout << "The wizard appears " << reaction << " by your action.";
}
void Wizard::DoInteraction_Bread()
{
    if (player.influences.Check(GetCollective()))
    {
        Item returnGift = ChooseRandom({ Item::Gold, Item::Potion });
        player.inventory.Add(returnGift);
        cout << "The wizard accepts the bread, gifting you 1 " << returnGift << " as thanks";
    }
    else
    {
        // Todo: make this less random?
        cout << "The wizard distrusts your bread, weary of assassination attempts from the " << collectives.RandomValue() << " collective.";
    }
}
void Wizard::DoInteraction_Sword()
{
    player.influences.Modify(GetCollective(), -1);
    cout << "\"Ouch!\" the wizard cries, disappearing into magical smoke.\n";
    cout << "As the wizard's robe falls to the ground, a potion falls from a pocket onto the floor.\n";
    if (player.luck.Apply(DiceCheck(1, 2)))
    {
        player.inventory.Add(Item::Potion);
        cout << "It lands gently on the robe and you pick it up, adding it to your inventory. It could be useful later.";
    }
    else
    {
        cout << "It smashes into a hundred pieces on the hard floor, the potion's effect activating instantly. "
            "You are engulfed in flames as the fire potion coats you like hot oil.\n";
        player.DoInteraction("potion", "fire");
    }
    Destroy();
}
void Wizard::DoInteraction_Gold()
{
    player.inventory.Add(Item::Potion);
    cout << "The wizard thanks you for the gold and gives you a potion.";
}

// Talk

void Wizard::DoInteraction_Talk_Generic()
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
