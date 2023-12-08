#include "utilities/pch/utilities.hpp"
#include "Door.hpp"
#include "entities/player/Player.hpp"
#include "helpers/Surroundings.hpp"
#include "helpers/echo.hpp"

void Door::DoInteraction_Grab( )
{
    echo << "The door opens, allowing you through.\n";
    surroundings.ReRoll( );
}
void Door::DoInteraction_Bread( )
{
    echo << "The door says nothing, but you get the odd sensation that it appriciates the gift.\n";
    player.GiveLuck(LuckType::Good);
}
void Door::DoInteraction_Sword( )
{
    if (DisadvantagedCoinFlip( ))
    {
        echo << "The door crumbles to bits and you walk through it.\n";
        surroundings.ReRoll( );
    }
    else
    {
        echo << "The door does not yield.\n";
    }
}
void Door::DoInteraction_Gold( )
{
    echo << "The door remains silent and unmoved. It has no concept for money.\n";
}

// Talk

void Door::DoInteraction_Talk(Topic topic)
{
    echo << "The door listens silently.\n";
}

// Potion

void Door::DoInteraction_Potion(Potion potion)
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
