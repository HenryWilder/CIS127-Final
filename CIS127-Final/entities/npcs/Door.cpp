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

void Door::DoInteraction_Talk([[maybe_unused]] Topic topic)
{
    echo << "The door listens silently.\n";
}

// Potion

void Door::DoInteraction_Potion(Potion potion)
{
    switch (potion)
    {
        case Potion::Predict:
            echo << "The door suddenly knows all of the people who are going to walk through it.\n";
            break;
        case Potion::Heal:
            echo << "The cracks in the door's oak body seemingly fill in as it is revitalized.\n";
            break;
        case Potion::Water:
            echo << "Hopefully that doesn't weaken it...\n";
            break;
        case Potion::Wish:
            echo << "The door has no wish.\n";
            break;
        case Potion::Ducks:
            echo << "The door does a good job of keeping the ducks out.\n";
            break;
        case Potion::Force:
            echo << "The door is blasted out of its frame.\n";
            break;
        case Potion::Salt:
            echo << "It isn't very effective.\n";
            break;
        case Potion::Ants:
            echo << "The door does a good job of keeping the ants out.\n";
            break;
        case Potion::Demon:
            echo << "[todo]"; // TODO: Implement Door demon potion interaction
            break;
        case Potion::Fire:
            echo << "The door burns slowly, crumbling to ash.\n";
            break;
        case Potion::Explode:
            echo << "With a flash, the door disintegrates.\n";
            break;
        case Potion::Tree:
            echo << "The door is already wood.\n";
            break;
    }
}
