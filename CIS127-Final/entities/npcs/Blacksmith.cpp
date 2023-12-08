#include "utilities/pch/utilities.hpp"
#include "Blacksmith.hpp"
#include "entities/player/Player.hpp"
#include "helpers/Surroundings.hpp"
#include "helpers/echo.hpp"
#include "utilities/randomness.hpp"

void Blacksmith::DoInteraction_Grab( )
{
    echo << "The blacksmith gives you a funny look, and asks if there was something you wanted.\n";
}

void Blacksmith::DoInteraction_Bread( )
{
    if (player.CheckInfluence(GetCollective( )))
    {
        auto [returnGift, quantity] = ChooseRandom(pair{ Item::Gold, 1 }, pair{ Item::Sword, 10 });
        echo << "The blacksmith accepts the bread, gifting you some " << returnGift << " as thanks.\n";
        player.AddItem(returnGift, quantity);
    }
    else
    {
        echo << "The blacksmith believes your bread is far too stale, proving their point by sharpening it into a sword and using it to slice another sword.\n";
    }
}

void Blacksmith::DoInteraction_Sword( )
{
    player.ModifyInfluence(GetCollective( ), -1);
    echo << "Without hesitation, the blacksmith meets your slash with a parry from their own sword.\n"
        "Your sword flies out of your hand and shatters against the wall, leaving you unarmed.\n";
    player.RemoveAllOfItem(Item::Sword);
}

void Blacksmith::DoInteraction_Gold( )
{
    echo << "The blacksmith thanks you for the gold and adds some durability to your sword.\n";
    player.AddItem(Item::Sword, 10);
}

// Talk

void Blacksmith::DoInteraction_Talk(Topic topic)
{
    // The blacksmith is a big intimidating person (of indeterminite gender) who doesn't talk much and comes off as cold.
    // The player is a knight (whose gender isn't indeterminite, but shouldn't be made explicit) in heavy plate armor covering their full body.
    // The player doesn't talk in this function.
    // The `topics` parameter is ignored in this override of the function.
    echo << "[todo]";
}

// Potion

void Blacksmith::DoInteraction_Potion(Potion potion)
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











