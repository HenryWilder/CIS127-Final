#include "utilities.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include "PlayerComponents.hpp"
#include "randomness.hpp"
#include "Prompt.hpp"
#include "TurnEchoStream.hpp"

// IMPORTANT:
// All interactions defined in this file are made with the assertion that player is targeting "self".

void Player::DoInteraction_Grab()
{
    echo << "...Weirdo.";
}
void Player::DoInteraction_Bread()
{
    Heal(1);
    echo << "It was delicious, replenishing 1 point of health.";
}
void Player::DoInteraction_Sword()
{
    switch (TestLuck())
    {
    case LuckType::Good:
        echo << "Fortunately, your armor is strong enough and your sword so cumbersome at such an angle that you barely scuffed yourself.";
        break;

    case LuckType::Neutral:
        Damage(1);
        echo << "You took 1 point of damage, dummy.";
        break;

    case LuckType::Bad:
        Damage(999);
        echo << "What did you expect was going to happen?";
        break;
    }
}
void Player::DoInteraction_Gold()
{
    AddItem(Item::Gold);
    echo << "You make a show of thanking yourself kindly for the gold before returning it to your money pouch.";
}

// Talk

string ContextualOpinion()
{
    return ChooseRandom(
        "may have overlooked",
        "exaggerated",
        "are overestimating",
        "underestimate",
        "may forget",
        "fail to appriciate",
        "are perpetuating harmful propaganda regarding",
        "make a good point about");
}

void NearbyThinksYou(const vector<string>& contextualResponses)
{
    echo << "The " << surroundings.Random() << " thinks you " << ContextualOpinion() << " " << ChooseRandom(contextualResponses) << ".";
}

void Player::DoInteraction_Talk_Generic()
{
    if (surroundings.IsEmpty())
    {
        echo << ChooseRandom({ "Fortunately", "Sadly" }) << ", nobody was around to hear it...";
        return;
    }
    
    const vector<string> possibleResponses = {
        "found it rather odd",
        "thinks you might need a checkup",
        "considers your points rather thought-provoking and insightful, if a little one-sided",
        "is too distracted by your utterly repulsive views to care that you were talking to a stone wall",
        "is confused who you were talking to",
        "looked around to check if there were any hidden familiars you may have been putting on a performance for",
        "gave you a weird look",
        "thinks you underestimate the effectiveness of brute force",
    };
    
    // It is by design that the responder might be inanimate. I thought it would be funny.
    echo << "The " << surroundings.Random() << " " << ChooseRandom(possibleResponses) << ".";
}
void Player::DoInteraction_Talk_WineFish()
{
    vector<string> contextualResponses = {
        "the resiliance of aquatic livers",
        "the work ethic of drunk merfolk",
        "how that would affect the wine market",
    };
    NearbyThinksYou(contextualResponses);
}
void Player::DoInteraction_Talk_SkeleStock()
{
    vector<string> contextualResponses = {
        "the staying power of good steel",
        "the dwindling numbers of the skeleton army",
        "how important armor is to the health of the Skeleton Alliance's trade sector",
    };
    NearbyThinksYou(contextualResponses);
}
void Player::DoInteraction_Talk_WoodpeckerSiege()
{
    vector<string> contextualResponses = {
        "an average woodpecker's endurance",
        "the fact that a woodpecker pecks wood, not stone",
        "the effectiveness of hot oil on an army of woodpeckers",
    };
    NearbyThinksYou(contextualResponses);
}
void Player::DoInteraction_Talk_SmithTeleken()
{
    vector<string> contextualResponses = {
        "the power of hydrokenesis when used to cool hot steel",
        "the craftsmanship needed to shape tools with one's mind",
        "the quality of a blade heated with telekenetic precision",
    };
    NearbyThinksYou(contextualResponses);
}
void Player::DoInteraction_Talk_PenguinBattle()
{
    vector<string> contextualResponses = {
        "what happened the last six times the Penguin Guild and the Old Realm got into a fight like this in the past",
        "just how much DNA the Penguin Guild's members actually share with real penguins",
        "how prone the Old Realm's shadow government is to shooting themselves in the foot when it comes to matters like this",
    };
    NearbyThinksYou(contextualResponses);
}
void Player::DoInteraction_Talk_Woodchuck()
{
    vector<string> contextualResponses = {
        "where they'd even get all that wood",
        "how long a woodchuck's arms are",
        "the possibility of seven or more woodchucks banding together and forming a woodchuck mega-fusion",
    };
    NearbyThinksYou(contextualResponses);
}

// Potion

void Player::DoInteraction_Potion_Predict()
{
    GiveLuck(LuckType::Good, 5);
    echo << "You recieve foresight of everything that will happen in the near future, and plan accordingly.";
}
void Player::DoInteraction_Potion_Heal()
{
    Heal(1);
    echo << "You felt rejuvenated, recovering 1 point of health.";
}
void Player::DoInteraction_Potion_Water()
{
    // Todo: status effect
    echo << "It soothes your wounds. Hopefully this doesn't rust your armor.";
}
void Player::DoInteraction_Potion_Wish()
{
    echo << "A genie appears from the the potion bottle.";

    Wish wish = wishes.Prompt("\"What would you like for your wish, " + GetName() + "?\"");
    echo << "The genie pauses for a moment before replying, \"Granted.\"\n";

    CollectiveInfo_t randCollective = collectives.Random();

    auto MaybeAddBabykickerNote = [&]()
    {
        bool isBabykickerCollective = randCollective == Collective::BabypunchingPuppykickers;
        bool isFirstTime = GetInfluence(Collective::BabypunchingPuppykickers) == 1; // Don't get repetitious.
        if (isBabykickerCollective && isFirstTime)
        {
            echo << '\n' << COLLECTIVE_BABYPUNCHING_PUPPYKICKERS_NOTE;
        }
    };

    switch (wish)
    {
    case Wish::Wealth:
        AddItem(Item::Gold, 3);
        echo << "You feel your money pouch get a little heavier.";
        break;

    case Wish::Health:
        // There is someone else to grant health to, *and* the genie is feeling silly
        if (!surroundings.IsEmpty() && CoinFlip())
        {
            // Other characters don't actually heal. NPC health is weird in this game.
            echo << "\"You wished for health, you didn't specify for whom\"\nYou notice the "
                 << surroundings.Random() << " appears to have regained some health...";
        }
        // Heal the player
        else
        {
            Heal(2);
            echo << "You feel your wounds heal, replenishing 2 health points.";
        }
        break;

    case Wish::Power:
        // Interpret power as political influence
        if (CoinFlip())
        {
            ModifyInfluence(randCollective, +1);
            echo << "You feel an inexplicable sensation of political influence over the " << randCollective.full << ".";
            MaybeAddBabykickerNote();
        }
        // Interpret power as physical strength
        else
        {
            AddItem(Item::Sword, 10);
            echo << "Your blade seems to magically sharpen itself, regaining 10 durability.";
        }
        break;

    case Wish::Status:
        ModifyInfluence(randCollective, +1);
        echo << "You feel an inexplicable sensation of high status among the " << randCollective.full << ".";
        MaybeAddBabykickerNote();
        break;

    case Wish::Luck:
        GiveLuck(LuckType::Good, 5);
        echo << "Nothing seems to have changed. You aren't even sure if the wish did anything.";
        break;

    case Wish::Faith:
        ModifyInfluence(randCollective, +1);
        echo << "You feel an inexplicable sensation of religious influence over the " << randCollective.full << ".";
        MaybeAddBabykickerNote();
        break;
    }

    echo << "\nThe genie disappears.";
}
void Player::DoInteraction_Potion_Ducks()
{
    echo << "A dozen ducks pop into existence around you. They seem lost and disoriented for a moment before waddling northeast towards the nearest pond.";
}
void Player::DoInteraction_Potion_Force()
{
    surroundings.ReRoll();
    echo << "[todo]";
}
void Player::DoInteraction_Potion_Salt()
{
    echo << "Your wounds begin burning and you feel dehydrated.";
}
void Player::DoInteraction_Potion_Ants()
{
    echo << "[todo]";
}
void Player::DoInteraction_Potion_Demon()
{
    vector<string> demonTypes = {
        "imp",
        "warewolf",
        "tentacle",
        "vampire",
    };
    string demon = ChooseRandom(demonTypes);
    echo << "You are surprised by the sudden appearance of " << (isvowel(demon[0]) ? "an" : "a") << " " << demon << ". ";
    
    if (demon == "imp")
    {
        Damage(2);
        echo << "The imp seems rather annoyed, throwing a fireball at you before de-summoning itself. You take 2 points of damage.";
    }
    else if (demon == "warewolf")
    {
        switch (CheckLuck())
        {
        case LuckType::Good:
            echo << "The warewolf seems friendly, and you get a chance to play fetch with it before it disappears in a puff of smoke.";
            break;

        case LuckType::Neutral:
        case LuckType::Bad:
        default:
            Damage(2);
            echo << "The warewolf growls at you and bites your arm before disappearing in a puff of smoke. "
                    "You are protected from most of the attack thanks to your armor, but it still takes away 2 points of your health.";
            break;
        }
    }
    else if (demon == "tentacle")
    {
        Damage(1);
        echo << "The tentacle slaps you violently, causing you to spin around in place before it slithers back down into its hole, disappearing from view. "
                "You lost 1 point of health.";
    }
    else if (demon == "vampire")
    {
        switch (CheckLuck())
        {
        case LuckType::Good:
            AddItem(Item::Bread, 3);
            echo << "The vampire seemed confused as to why they were summoned, "
                    "but decides now is as decent a time as any to dispose of the garlic bread people keep throwing at them. "
                    "They " << ChooseRandom({ "plop", "stuff", "drop", "place" }) << " the bread into your hand and promptly transform into a small black bat, "
                    "flying away into the darkness.";
            break;

        case LuckType::Neutral:
            echo << "The vampire seems frustrated by your frivolous summoning and kicks you in your armored shin before transforming into a bat and gliding off into the night. "
                    "You take no damage, but it still stings.";
            break;

        case LuckType::Bad:
            Damage(1);
            echo << "The vampire seems frustrated by your frivolous summoning and kicks you in your armored shin before transforming into a bat and gliding off into the night. "
                    "The kick was surprisingly hard, taking 1 point of your health.";
            break;
        }
    }
}
void Player::DoInteraction_Potion_Fire()
{
    Damage(1);
    ApplyStatusEffect(StatusEffects::Fire);
    echo << "It hurt quite a bit, taking away 1 point of health.";
}
void Player::DoInteraction_Potion_Explode()
{
    Damage(3);
    if (CoinFlip())
    {
        ApplyStatusEffect(StatusEffects::Fire);
    }
    echo << "You feel your armor heat up tremendously, practically baking you within it. You feel 3 health points drain from your soul.";
}
void Player::DoInteraction_Potion_Tree()
{
    echo << "You transform into a tree.";
}

const string& Player::GetName() const
{
    return name;
}

void Player::Init()
{
    name = PromptLine("What's your name?");
    Inventory     :: Init( );
    Influences    :: Init( );
    Health        :: Init( );
    StatusEffects :: Init( );
    Luck          :: Init( );
}

void Player::Save(ostream& ofs) const
{
    ofs << "name: " << name << '\n';
    Inventory     :: Save(ofs);
    Influences    :: Save(ofs);
    Health        :: Save(ofs);
    StatusEffects :: Save(ofs);
    Luck          :: Save(ofs);
}

void Player::Load(istream& ifs)
{
    ifs.ignore(16, ':').ignore(1, ' '); // Need to explicitly ignore the space as well due to getline
    getline(ifs, name);
    Inventory     :: Load(ifs);
    Influences    :: Load(ifs);
    Health        :: Load(ifs);
    StatusEffects :: Load(ifs);
    Luck          :: Load(ifs);
}

// Global instance of the player so that other entities can interact with it
Player player;
