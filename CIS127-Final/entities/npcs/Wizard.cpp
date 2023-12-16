#include "utilities/pch/utilities.hpp"
#include "Wizard.hpp"
#include "entities/player/Player.hpp"
#include "helpers/Surroundings.hpp"
#include "helpers/echo.hpp"

void Wizard::DoInteraction_Grab( )
{
    string reaction = DiceCheck(1, 2)
        ? ChooseRandom("surprised", "intrigued", "embarrassed", "amazed", "curious")
        : ChooseRandom("surprised", "unamused", "annoyed", "baffled", "disappointed");
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
        echo << "It smashes into a hundred pieces on the hard floor, the potion's effect activating instantly. ";
        echo << "You are engulfed in flames as the fire potion coats you like hot oil.\n";
        player.DoInteraction_Potion(Potion::Fire);
    }
}

void Wizard::DoInteraction_Gold( )
{
    echo << "The wizard thanks you for the gold and gives you a potion.\n";
    player.AddItem(Item::Potion);
}

// Talk

void Wizard::DoInteraction_Talk([[maybe_unused]] Topic topic)
{
    echo << "The wizard seems to " << ChooseRandom("agree", "be agitated") << " with your views.\n";
}

// Potion

void Wizard::DoInteraction_Potion(Potion potion)
{
    switch (potion)
    {
        case Potion::Predict:
            echo << "The wizard already uses their crystal ball on enough of a regular basis that they didn't learn anything new.\n";
            break;
        case Potion::Heal:
            echo << "The wizard snatches the potion out of the air and pockets it for later. They toss you a gold coin in return.\n";
            player.AddItem(Item::Gold);
            break;
        case Potion::Water:
            echo << "The wizard waves a hand, guiding the water back into your face.\n";
            echo << "You are all wet.\n";
            break;
        case Potion::Wish:
            echo << "The wizard's cloak seems to get a little longer.\n";
            break;
        case Potion::Ducks:
            echo << "The wizard shouts, their voice echoing, \"HUZZAH! BONUS DUCKS!!\". They seem happy about the ducks.\n";
            break;
        case Potion::Force:
            echo << "The wizard's hat gets blown off their head and they chase after it into another room.\n";
            RemoveFromWorld();
            break;
        case Potion::Salt:
        {
            string what = ChooseRandom("cooking", "rituals", "potion-making");
            echo << "The wizard thanks you for the salt, storing it in a pouch to use in " << what << " later.\n";
            break;
        }
        case Potion::Ants:
            echo << "The wizard levitates the ants into a ball and 'poofs' them far away.\n";
            break;
        case Potion::Demon:
            echo << "[todo]"; // TODO: Implement Wizard demon potion interaction
            break;
        case Potion::Fire:
            echo << "The wizard waves a hand and redirects the flames into a small jar on their hip. The jar glows, providing heat with the swirling flames contained within it.\n";
            break;
        case Potion::Explode:
            echo << "The wizard deflects the explosion, sending it back at you.\n";
            player.Damage(3);
            break;
        case Potion::Tree:
            echo << "The wizard transforms into a tall, skinny tree. They look a lot less colorful now without their pretty robes...";
            break;
    }
}











