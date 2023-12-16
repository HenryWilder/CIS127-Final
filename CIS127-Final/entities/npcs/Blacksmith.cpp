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

void Blacksmith::DoInteraction_Talk([[maybe_unused]] Topic topic)
{
    echo << "The blacksmith grunts.";
}

// Potion

void Blacksmith::DoInteraction_Potion(Potion potion)
{
    switch (potion)
    {
        case Potion::Predict:
        {
            string what = ChooseRandom(
                "next year's trends in blade ornamentation",
                "next week's lottery numbers"
            );
            echo << "The blacksmith seems to have a blast of inspiration, writing down a note regarding " << what << ".\n";
            break;
        }
        case Potion::Heal:
            echo << "The blacksmith didn't like being splashed, but appriciates the healing.\n";
            if (CoinFlip()) echo << "You notice the calluses on their hands seem to have faded.\n";
            break;
        case Potion::Water:
        {
            string expression = ChooseRandom("disapproval", "frustration");
            echo << "The blacksmith doesn't seem to react, but give an expression of " << expression << ".\n";
            break;
        }
        case Potion::Wish:
            echo << "The hammer on the blacksmith's belt seems to get slightly bigger.\n";
            break;
        case Potion::Ducks:
            echo << "The blacksmith watches stoically as ducks waddle around them.\n";
            if (CoinFlip()) echo << "One sits on their shoulder and quacks. The blacksmith seems to tolerate it.\n";
            break;
        case Potion::Force:
            echo << "The blacksmith is unmoved, held in place by their impressive mass.\n";
            break;
        case Potion::Salt:
            echo << "They seem a little bugged by the salt, but nothing more.\n";
            break;
        case Potion::Ants:
            echo << "The blacksmith starts scratching themselves all over, frantically ridding themself of the ants.\n";
            break;
        case Potion::Demon:
            echo << "[todo]"; // TODO: Implement Blacksmith demon potion interaction
            break;
        case Potion::Fire:
            echo << "A flame consumes the blacksmith. They seem unaffected by it; used to the heat.\n";
            break;
        case Potion::Explode:
            echo << "A puff of smoke filles the room. The blacksmith is somehow unaffected, other that being covered in soot.\n";
            echo << "You remind yourself not to mess with them...\n";
            break;
        case Potion::Tree:
            echo << "The blacksmith transforms into a tree. They don't seem that different...\n";
            break;
    }
}
