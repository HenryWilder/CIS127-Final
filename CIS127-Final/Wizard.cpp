#include "utilities.hpp"
#include "Wizard.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include "TurnEchoStream.hpp"

void Wizard::DoInteraction_Grab( )
{
    string reaction, actionDescriptor;
    if (DiceCheck(1, 2))
    {
        reaction = ChooseRandom("surprised", "intrigued", "embarrassed");
    }
    else
    {
        reaction = ChooseRandom("surprised", "unamused", "annoyed");
    }
    echo << "The wizard appears " << reaction << " by your action.\n";
}
void Wizard::DoInteraction_Bread( )
{
    if (player.CheckInfluence(GetCollective( )))
    {
        Item returnGift = ChooseRandom(Item::Gold, Item::Potion);
        echo << "The wizard accepts the bread, gifting you 1 " << returnGift << " as thanks.\n";
        player.AddItem(returnGift);
    }
    else
    {
        // Todo: make this less random?
        echo << "The wizard distrusts your bread, weary of assassination attempts from the " << collectives.RandomValue( ) << " collective.\n";
    }
}
void Wizard::DoInteraction_Sword( )
{
    echo << "\"Ouch!\" the wizard cries, disappearing into magical smoke.\n";
    RemoveFromWorld( );
    player.ModifyInfluence(GetCollective( ), -1);
    echo << "As the wizard's robe falls to the ground, a potion falls from a pocket onto the floor.\n";
    if (player.ApplyLuck(CoinFlip( )))
    {
        echo << "It lands gently on the robe and you pick it up, adding it to your inventory. It could be useful later.\n";
        player.AddItem(Item::Potion);
    }
    else
    {
        echo << "It smashes into a hundred pieces on the hard floor, the potion's effect activating instantly. "
            "You are engulfed in flames as the fire potion coats you like hot oil.\n";
        player.DoInteraction_Potion(Potion::Fire);
    }
}
void Wizard::DoInteraction_Gold( )
{
    echo << "The wizard thanks you for the gold and gives you a potion.\n";
    player.AddItem(Item::Potion);
}

// Talk

void Wizard::DoInteraction_Talk_Generic( )
{
    echo << "[todo]\n";
}

// Potion

void Wizard::DoInteraction_Potion(Potion potion)
{
    switch (potion)
    {
        case Potion::Predict:
            echo << "[todo]";
            break;
        case Potion::Heal:
            echo << "[todo]";
            break;
        case Potion::Water:
            echo << "[todo]";
            break;
        case Potion::Wish:
            echo << "[todo]";
            break;
        case Potion::Ducks:
            echo << "[todo]";
            break;
        case Potion::Force:
            echo << "[todo]";
            break;
        case Potion::Salt:
            echo << "[todo]";
            break;
        case Potion::Ants:
            echo << "[todo]";
            break;
        case Potion::Demon:
            echo << "[todo]";
            break;
        case Potion::Fire:
            echo << "[todo]";
            break;
        case Potion::Explode:
            echo << "[todo]";
            break;
        case Potion::Tree:
            echo << "[todo]";
            break;
    }
}











