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
    echo << "[todo]"; // TODO: Implement Blacksmith talk interaction
}

// Potion

void Blacksmith::DoInteraction_Potion(Potion potion)
{
    switch (potion)
    {
        case Potion::Predict:
            echo << "[todo]"; // TODO: Implement Blacksmith predict potion interaction
            break;
        case Potion::Heal:
            echo << "[todo]"; // TODO: Implement Blacksmith heal potion interaction
            break;
        case Potion::Water:
            echo << "[todo]"; // TODO: Implement Blacksmith water potion interaction
            break;
        case Potion::Wish:
            echo << "[todo]"; // TODO: Implement Blacksmith wish potion interaction
            break;
        case Potion::Ducks:
            echo << "[todo]"; // TODO: Implement Blacksmith ducks potion interaction
            break;
        case Potion::Force:
            echo << "[todo]"; // TODO: Implement Blacksmith force potion interaction
            break;
        case Potion::Salt:
            echo << "[todo]"; // TODO: Implement Blacksmith salt potion interaction
            break;
        case Potion::Ants:
            echo << "[todo]"; // TODO: Implement Blacksmith ants potion interaction
            break;
        case Potion::Demon:
            echo << "[todo]"; // TODO: Implement Blacksmith demon potion interaction
            break;
        case Potion::Fire:
            echo << "[todo]"; // TODO: Implement Blacksmith fire potion interaction
            break;
        case Potion::Explode:
            echo << "[todo]"; // TODO: Implement Blacksmith explode potion interaction
            break;
        case Potion::Tree:
            echo << "[todo]"; // TODO: Implement Blacksmith tree potion interaction
            break;
    }
}











