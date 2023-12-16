#include "utilities/pch/utilities.hpp"
#include "Baker.hpp"
#include "utilities/randomness.hpp"
#include "entities/player/Player.hpp"
#include "helpers/Surroundings.hpp"
#include "helpers/echo.hpp"

void Baker::DoInteraction_Grab( )
{
    echo << "The baker seems a bit annoyed at your advances. They suggest that if you wanted some bread you could just ask.\n";
}
void Baker::DoInteraction_Bread( )
{
    if (player.CheckInfluence(GetCollective( )))
    {
        echo << "The baker thanks you for the bread and gives you some gold in return.\n";
    }
    else if (player.CountItem(Item::Gold) >= 2)
    {
        echo << "The baker thanks you for the bread and sells it back to you.\n";
        player.TryRemoveItemQty(Item::Gold, 2);
        player.AddItem(Item::Bread);
    }
    else
    {
        echo << "The baker accepts the bread.\n";
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
    echo << "The baker doesn't know about all that, but offers you a cookie for being so cheerful.\n";
}

// Potion

void Baker::DoInteraction_Potion(Potion potion)
{
    switch (potion)
    {
        case Potion::Predict:
            echo << "The baker is struck with a preminition of exactly when the bread they currently have baking will be done. It isn't very helpful, they already knew that...\n";
            break;
        case Potion::Heal:
            echo << "The baker wasn't hurt to begin with, but it seems to clean them up a little nonetheless.\n";
            break;
        case Potion::Water:
        {
            string feeling = ChooseRandom("frustrated", "irritated" "annoyed", "agitated", "disheartened");
            echo << "The baker groans, feeling " << feeling << " as they clean up the sticky paste of flour on their hands and apron.\n";
            isCoveredInPaste = true;
            break;
        }
        case Potion::Wish:
        {
            int count = RandomNumber(1000, 5000);
            echo << count << " loafs of bread " << ChooseRandom("poof into existence", "appear out of thin air") << ". You pick some of them up.\n";
            player.AddItem(Item::Bread, count / 4);
            break;
        }
        case Potion::Ducks:
            echo << "The baker offers the ducks some of yesterday's bread. They seem to enjoy it, and this won't become a frequent habbit for them.\n";
            break;
        case Potion::Force:
        {
            string feeling = ChooseRandom("fear", "amazement", "confusion", "dismay");
            echo << "The baker flops onto the floor, their eyes wide in " << feeling << " as they are rolled out of the room.\n";
            RemoveFromWorld();
            break;
        }
        case Potion::Salt:
            echo << "The baker scoops up the salt and saves it for later.\n";
            break;
        case Potion::Ants:
            echo << "The baker does not appriciate being swarmed with ants.\n";
            break;
        case Potion::Demon:
            echo << "[todo]\n"; // TODO: Implement Baker demon potion interaction
            break;
        case Potion::Fire:
            if (isCoveredInPaste)
            {
                string feeling = ChooseRandom("exasperation", "resignation", "dismay", "perturbation", "vexation");
                echo << "The baker shouts in " << feeling << " as the wet paste covering their apron and hands dries and hardens into a crust.\n";
                echo << "The substance remains stuck to their clothes, limiting their movement.\n";
                echo << "This will be quite difficult to clean up...\n";
            }
            else
            {
                echo << "The baker " << ChooseRandom("screams", "squeals", "shouts") << " in pain as the flames nibble them from head to toe.\n";
            }
            break;
        case Potion::Explode:
            echo << "The baker pops like a bubble, a dozen loaves of bread spilling onto the floor. The loaves hover and rotate in place as you snatch them up.\n";
            player.AddItem(Item::Bread, 13);
            break;
        case Potion::Tree:
            echo << "The baker transforms into a short, fat tree. This will be rather bad for their baking business...\n";
            break;
    }
}
