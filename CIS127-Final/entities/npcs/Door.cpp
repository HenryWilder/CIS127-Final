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
            echo << "[todo]"; // TODO: Implement Door predict potion interaction
            break;
        case Potion::Heal:
            echo << "[todo]"; // TODO: Implement Door heal potion interaction
            break;
        case Potion::Water:
            echo << "[todo]"; // TODO: Implement Door water potion interaction
            break;
        case Potion::Wish:
            echo << "[todo]"; // TODO: Implement Door wish potion interaction
            break;
        case Potion::Ducks:
            echo << "[todo]"; // TODO: Implement Door ducks potion interaction
            break;
        case Potion::Force:
            echo << "[todo]"; // TODO: Implement Door force potion interaction
            break;
        case Potion::Salt:
            echo << "[todo]"; // TODO: Implement Door salt potion interaction
            break;
        case Potion::Ants:
            echo << "[todo]"; // TODO: Implement Door ants potion interaction
            break;
        case Potion::Demon:
            echo << "[todo]"; // TODO: Implement Door demon potion interaction
            break;
        case Potion::Fire:
            echo << "[todo]"; // TODO: Implement Door fire potion interaction
            break;
        case Potion::Explode:
            echo << "[todo]"; // TODO: Implement Door explode potion interaction
            break;
        case Potion::Tree:
            echo << "[todo]"; // TODO: Implement Door tree potion interaction
            break;
    }
}
