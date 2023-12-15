#include "utilities/pch/utilities.hpp"
#include "Baker.hpp"
#include "entities/player/Player.hpp"
#include "helpers/Surroundings.hpp"
#include "helpers/echo.hpp"

void Baker::DoInteraction_Grab( )
{
    echo << "[todo]\n";
}
void Baker::DoInteraction_Bread( )
{
    if (player.CheckInfluence(GetCollective( )))
    {
        echo << "The baker thanks you for the bread and gives you some gold in return.";
    }
    else if (player.CountItem(Item::Gold) >= 2)
    {
        echo << "The baker thanks you for the bread and sells it back to you.\n";
        player.TryRemoveItemQty(Item::Gold, 2);
        player.AddItem(Item::Bread);
    }
    else
    {
        echo << "The baker accepts the bread.";
    }
}
void Baker::DoInteraction_Sword( )
{
    switch (player.TestLuck( ))
    {
        case LuckType::Good:
            echo << "The baker crumbles into a baker's dozen loafs of bread that scatter across the floor. You rapidly snatch up each one.\n";
            player.AddItem(Item::Bread, 13);
            RemoveFromWorld( );
            break;

        case LuckType::Neutral:
            echo << "The baker didn't much enjoy that, but they're gone now.\n";
            RemoveFromWorld( );
            break;

        case LuckType::Bad:
            echo << "You have angered the bread gods.\n";
            break;
    }
}
void Baker::DoInteraction_Gold( )
{
    echo << "The baker thanks you for the gold and hands you some bread.\n";
    player.AddItem(Item::Bread);
}

// Baker

void Baker::DoInteraction_Talk(Topic topic)
{
    echo << "[todo]\n"; // TODO: Implement Baker talk interaction
}

// Potion

void Baker::DoInteraction_Potion(Potion potion)
{
    switch (potion)
    {
        case Potion::Predict:
            echo << "[todo]\n"; // TODO: Implement Baker predict potion interaction
            break;
        case Potion::Heal:
            echo << "[todo]\n"; // TODO: Implement Baker heal potion interaction
            break;
        case Potion::Water:
            echo << "[todo]\n"; // TODO: Implement Baker water potion interaction
            break;
        case Potion::Wish:
            echo << "[todo]\n"; // TODO: Implement Baker wish potion interaction
            break;
        case Potion::Ducks:
            echo << "[todo]\n"; // TODO: Implement Baker ducks potion interaction
            break;
        case Potion::Force:
            echo << "[todo]\n"; // TODO: Implement Baker force potion interaction
            break;
        case Potion::Salt:
            echo << "[todo]\n"; // TODO: Implement Baker salt potion interaction
            break;
        case Potion::Ants:
            echo << "[todo]\n"; // TODO: Implement Baker ants potion interaction
            break;
        case Potion::Demon:
            echo << "[todo]\n"; // TODO: Implement Baker demon potion interaction
            break;
        case Potion::Fire:
            echo << "[todo]\n"; // TODO: Implement Baker fire potion interaction
            break;
        case Potion::Explode:
            echo << "[todo]\n"; // TODO: Implement Baker explode potion interaction
            break;
        case Potion::Tree:
            echo << "[todo]\n"; // TODO: Implement Baker tree potion interaction
            break;
    }
}
