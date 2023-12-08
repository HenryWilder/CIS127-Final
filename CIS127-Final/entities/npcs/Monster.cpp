#include "utilities/pch/utilities.hpp"
#include "Monster.hpp"
#include "entities/player/Player.hpp"
#include "helpers/Surroundings.hpp"
#include "helpers/echo.hpp"

void Monster::DoInteraction_Grab( )
{
    bool isPositive = player.CheckInfluence(Collective::Monsters);


    string reaction, reactingTo, untilRelease;
    if (isPositive)
    {
        reaction = ChooseRandom("surprised", "impressed", "confused", "baffled", "amused", "astonished", "flustered", "bewildered");
        reactingTo = ChooseRandom("forwardness", "strength", "agility", "fearlessness", "courage");
    }
    else
    {
        reaction = ChooseRandom("surprised", "confused", "unamused", "annoyed", "upset", "angered");
        reactingTo = ChooseRandom("foolishness", "lack of self-preservation", "disrespect");
    }
    untilRelease = ChooseRandom(
        "staring at you",
        "watching you with curiosity");

    echo << "The monster seems " << reaction << " by your " << reactingTo << ", " << untilRelease << " until you release it from your grip.\n";


    if (reaction == "flustered" || ((reaction == "impressed" || reaction == "surprised") && reactingTo == "forwardness"))
    {
        echo << "Upon being released, it discretely slips something into the metal breastplate pocket of your armor.\n";
        player.AddItem(Item::Phonenumber);
    }
    else if (isPositive)
    {
        string response = ChooseRandom(
            "gives a little nod of respect",
            "makes the tiniest of smirks",
            "shows off its teeth in what you can only imagine is a good way in its culture",
            "gives you what is meant to be a gentle pat on the head"
        );
        echo << "It " << response << ".\n";
        player.ModifyInfluence(Collective::Monsters, +1);
    }
    else // negative
    {
        echo << "It decides to return the favor, squeezing you in its massive claws as you feel your armor dent and tighten around your fragile, squishy body.\n";
        player.Damage(3);
    }
}
void Monster::DoInteraction_Bread( )
{
    bool isPositive = player.CheckInfluence(Collective::Monsters);

    echo << "The monster ";
    if (isPositive)
    {
        string reaction = ChooseRandom(
            "is delighted by",
            "is overjoyed at",
            "looks longingly at",
            "appriciates",
            "tears up over",
            "stares in surprise at",
            "curiously examines",
            "curiously investigates"
        );
        string response = ChooseRandom(
            "scarfing it down hungrily",
            "taking long, deliberate bites out of it",
            "tucking it away for later",
            "sheathing it on its back like a sword",
            "sliding the entire loaf down its neck in one bite",
            "slipping the loaf into its conveniently-sized hat",
            "stuffing the loaf through what you had previously mistaken for a cycloptic eye",
            "letting the bread melt and assimilate into its smooth, featureless face"
        );
        string gratitude = ChooseRandom(
            "nodding its head with what is meant to be a genuine smile, though it has difficulty trying not to look frightening",
            "giving you what is meant to be a pat on the head; unintentionally causing your helmet to clang against the neck of your breastplate",
            "wrapping its long, heavy arms around you in thanks. It drops you onto the floor after a few seconds, and you lie there stunned for a moment before managing to stand back up",
            "pulling you into a warm bear hug for a full thirty seconds before setting you down"
        );
        echo << reaction << " the gift, quietly accepting it and " << response << " before " << gratitude << ".\n";
        player.ModifyInfluence(Collective::Monsters, 1);
    }
    else
    {
        string reaction = ChooseRandom("scowels", "scoffs", "screams", "screeches", "foams", "growls", "hisses");
        echo << reaction << " at the very idea of accepting a gift from you, slapping it out of your hand and onto the floor.\n";
    }
}
void Monster::DoInteraction_Sword( )
{
    bool isSuccessful = player.ApplyLuck(DiceCheck(1, 2));
    if (isSuccessful)
    {
        string bodyCovering = ChooseRandom("gooey", "vaporous", "slimey", "meaty", "fuzzy", "fur-covered");
        echo << "Your sword slashes through the monster, splitting it into two " << bodyCovering << " chunks on the floor. "
            "A gold coin drops to the cold, hard floor with a clink. You pick it up, adding it to your collection as the body fizzles to dust.\n";
        player.AddItem(Item::Gold);
    }
    else
    {
        bool isSwordSurviving = player.CountItem(Item::Sword) >= 1;

        string reaction = ChooseRandom(
            "grabs your sword mid-swing, gripping it tightly in its claws",
            "swats your sword out of your hand, flinging it into the wall",
            "squeezes your sword's blade with two hands",
            "bites down on your sword violently with its massive shark-teeth");

        string impact;
        if (isSwordSurviving)
        {

            impact = ChooseRandom("slightly", "somewhat", "lightly", "partly", "gently"); // adverb
            impact += " ";
            impact += ChooseRandom("bending", "dulling", "denting", "melting", "cracking", "chipping"); // verb
            impact += " it";
        }
        else
        {
            impact = ChooseRandom("completely", "totally", "fully", "absolutely", "utterly", "instantly"); // adverb
            impact += " ";
            impact += ChooseRandom("shattering", "obliterating", "liquifying", "anihilating", "evaporating", "disintegrating", "digesting"); // verb
            impact += " everything above the hilt";
        }

        echo << "The monster " << reaction << " and " << impact << ".\n";
        player.TryRemoveItemQty(Item::Sword);
    }
}
void Monster::DoInteraction_Gold( )
{
    echo << "The monster doesn't know what to do with the gold, tilting its head curiously but quickly losing interest.\n";
}

// Talk

void Monster::DoInteraction_Talk(Topic topic)
{
    echo << "You feel the creature's many eyes burning through your soul.\n";
}

// Potion

void Monster::DoInteraction_Potion(Potion potion)
{
    switch (potion)
    {
        case Potion::Predict:
            echo << "The monster snarls, and you see in its eyes that it knows exactly what you are going to do next; even if it cannot prevent it.\n";
            hasForesight = true;
            break;
        case Potion::Heal:
        {
            const char *action = ChooseRandom("tilts its head", "gives a toothy grin", "growls", "oozes an acidic substance onto the floor");
            const char *reaction = CheckForesight( ) ? "having predicted that" : ChooseRandom("pleased that", "curious why", "confused that");
            echo << "The monster " << action << ", " << reaction << " you would heal it.\n";
            break;
        }
        case Potion::Water:
            echo << "The monster seems not to mind being wet in the slightest.\n";
            break;
        case Potion::Wish:
        {
            const char *wish = ChooseRandom(
                "world peace",
                "its parental figure's health",
                "the doom of humanity",
                "\"decay\"",
                "all things to rot",
                "eternal life",
                ""
            );
            echo << "You can't completely understand what it said. But from what little you know of its tongue, the monster seemed to wished for " << wish << "...\n";
            echo << "Considering nothing changed, you assume the wish is beyond the scope of a single potion.\n";
            break;
        }
        case Potion::Ducks:
            echo << "[todo]"; // TODO: Implement Monster ducks potion interaction
            break;
        case Potion::Force:
            echo << "[todo]"; // TODO: Implement Monster force potion interaction
            break;
        case Potion::Salt:
            echo << "[todo]"; // TODO: Implement Monster salt potion interaction
            break;
        case Potion::Ants:
            echo << "[todo]"; // TODO: Implement Monster ants potion interaction
            break;
        case Potion::Demon:
            echo << "[todo]"; // TODO: Implement Monster demon potion interaction
            break;
        case Potion::Fire:
            echo << "[todo]"; // TODO: Implement Monster fire potion interaction
            break;
        case Potion::Explode:
            echo << "[todo]"; // TODO: Implement Monster explode potion interaction
            break;
        case Potion::Tree:
            echo << "[todo]"; // TODO: Implement Monster tree potion interaction
            break;
    }
}
