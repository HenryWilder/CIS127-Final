#include "Wizard.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include "TurnEchoStream.hpp"

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
    echo << "The wizard appears " << reaction << " by your action.";
}
void Wizard::DoInteraction_Bread()
{
    if (player.influences.Check(GetCollective()))
    {
        Item returnGift = ChooseRandom(Item::Gold, Item::Potion);
        player.inventory.Add(returnGift);
        echo << "The wizard accepts the bread, gifting you 1 " << returnGift << " as thanks";
    }
    else
    {
        // Todo: make this less random?
        echo << "The wizard distrusts your bread, weary of assassination attempts from the " << collectives.RandomValue() << " collective.";
    }
}
void Wizard::DoInteraction_Sword()
{
    player.influences.Modify(GetCollective(), -1);
    echo << "\"Ouch!\" the wizard cries, disappearing into magical smoke.\n";
    echo << "As the wizard's robe falls to the ground, a potion falls from a pocket onto the floor.\n";
    if (player.luck.Apply(DiceCheck(1, 2)))
    {
        player.inventory.Add(Item::Potion);
        echo << "It lands gently on the robe and you pick it up, adding it to your inventory. It could be useful later.";
    }
    else
    {
        echo << "It smashes into a hundred pieces on the hard floor, the potion's effect activating instantly. "
            "You are engulfed in flames as the fire potion coats you like hot oil.\n";
        player.DoInteraction_Potion(Potion::Fire);
    }
    RemoveFromWorld();
}
void Wizard::DoInteraction_Gold()
{
    player.inventory.Add(Item::Potion);
    echo << "The wizard thanks you for the gold and gives you a potion.";
}

// Talk

void Wizard::DoInteraction_Talk_Generic()
{
    echo << "[todo]";
}

// Potion

void Wizard::DoInteraction_Potion_Predict()
{
    echo << "[todo]";
}
void Wizard::DoInteraction_Potion_Heal()
{
    echo << "[todo]";
}
void Wizard::DoInteraction_Potion_Water()
{
    echo << "[todo]";
}
void Wizard::DoInteraction_Potion_Wish()
{
    echo << "[todo]";
}
void Wizard::DoInteraction_Potion_Ducks()
{
    echo << "[todo]";
}
void Wizard::DoInteraction_Potion_Force()
{
    echo << "[todo]";
}
void Wizard::DoInteraction_Potion_Salt()
{
    echo << "[todo]";
}
void Wizard::DoInteraction_Potion_Ants()
{
    echo << "[todo]";
}
void Wizard::DoInteraction_Potion_Demon()
{
    echo << "[todo]";
}
void Wizard::DoInteraction_Potion_Fire()
{
    echo << "[todo]";
}
void Wizard::DoInteraction_Potion_Explode()
{
    echo << "[todo]";
}
void Wizard::DoInteraction_Potion_Tree()
{
    echo << "[todo]";
}
