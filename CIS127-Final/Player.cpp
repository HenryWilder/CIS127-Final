#include "Player.hpp"
#include "utilities.hpp"
#include "Surroundings.hpp"
#include "Components.hpp"
#include "randomness.hpp"
#include "Prompt.hpp"

// IMPORTANT:
// All interactions defined in this file are made with the assertion that player is targeting "self".

void Player::DoInteraction_Grab()
{
    cout << "...Weirdo.";
}
void Player::DoInteraction_Bread()
{
    health.Heal(1);
    cout << "It was delicious, replenishing 1 point of health.";
}
void Player::DoInteraction_Sword()
{
    switch (luck.Test())
    {
    case Luck::Good:
        cout << "Fortunately, your armor is strong enough and your sword so cumbersome at such an angle that you barely scuffed yourself.";
        break;

    case Luck::Neutral:
        health.Damage(1);
        cout << "You took 1 point of damage, dummy.";
        break;

    case Luck::Bad:
        health.Damage(999);
        cout << "What did you expect was going to happen?";
        break;
    }
}
void Player::DoInteraction_Gold()
{
    inventory.Add(Item::Gold);
    cout << "You make a show of thanking yourself kindly for the gold before returning it to your money pouch.";
}

// Talk

string ContextualOpinion()
{
    vector<string> contextualOpinion = {
        "may have overlooked",
        "exaggerated",
        "are overestimating",
        "underestimate",
        "may forget",
        "fail to appriciate",
        "are perpetuating harmful propaganda regarding",
        "make a good point about",
    };
    return ChooseRandom(contextualOpinion);
}

void NearbyThinksYou(const Surroundings& surroundings, vector<string> contextualResponses)
{
    cout << "The " << surroundings.Random() << " thinks you " << ContextualOpinion() << " " << ChooseRandom(contextualResponses) << ".";
}

void Player::DoInteraction_Talk_Generic()
{
    if (surroundings.IsEmpty())
    {
        cout << ChooseRandom({ "Fortunately", "Sadly" }) << ", nobody was around to hear it...";
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
    cout << "The " << surroundings.Random() << " " << ChooseRandom(possibleResponses) << ".";
}
void Player::DoInteraction_Talk_WineFish()
{
    vector<string> contextualResponses = {
        "the resiliance of aquatic livers",
        "the work ethic of drunk merfolk",
        "how that would affect the wine market",
    };
    NearbyThinksYou(surroundings, contextualResponses);
}
void Player::DoInteraction_Talk_SkeleStock()
{
    vector<string> contextualResponses = {
        "the staying power of good steel",
        "the dwindling numbers of the skeleton army",
        "how important armor is to the health of the Skeleton Alliance's trade sector",
    };
    NearbyThinksYou(surroundings, contextualResponses);
}
void Player::DoInteraction_Talk_WoodpeckerSiege()
{
    vector<string> contextualResponses = {
        "an average woodpecker's endurance",
        "the fact that a woodpecker pecks wood, not stone",
        "the effectiveness of hot oil on an army of woodpeckers",
    };
    NearbyThinksYou(surroundings, contextualResponses);
}
void Player::DoInteraction_Talk_SmithTeleken()
{
    vector<string> contextualResponses = {
        "the power of hydrokenesis when used to cool hot steel",
        "the craftsmanship needed to shape tools with one's mind",
        "the quality of a blade heated with telekenetic precision",
    };
    NearbyThinksYou(surroundings, contextualResponses);
}
void Player::DoInteraction_Talk_PenguinBattle()
{
    vector<string> contextualResponses = {
        "what happened the last six times the Penguin Guild and the Old Realm got into a fight like this in the past",
        "just how much DNA the Penguin Guild's members actually share with real penguins",
        "how prone the Old Realm's shadow government is to shooting themselves in the foot when it comes to matters like this",
    };
    NearbyThinksYou(surroundings, contextualResponses);
}
void Player::DoInteraction_Talk_Woodchuck()
{
    vector<string> contextualResponses = {
        "where they'd even get all that wood",
        "how long a woodchuck's arms are",
        "the possibility of seven or more woodchucks banding together and forming a woodchuck mega-fusion",
    };
    NearbyThinksYou(surroundings, contextualResponses);
}

// Potion

void Player::DoInteraction_Potion_Predict()
{
    luck.Give(Luck::Good, 5);
    cout << "You recieve foresight of everything that will happen in the near future, and plan accordingly.";
}
void Player::DoInteraction_Potion_Heal()
{
    health.Heal(1);
    cout << "You felt rejuvenated, recovering 1 point of health.";
}
void Player::DoInteraction_Potion_Water()
{
    // Todo: status effect
    cout << "It soothes your wounds. Hopefully this doesn't rust your armor.";
}
void Player::DoInteraction_Potion_Wish()
{
    cout << "A genie appears from the the potion bottle.";

    Wish wish = wishes.Prompt("\"What would you like for your wish, " + GetName() + "?\"");
    cout << "The genie pauses for a moment before replying, \"Granted.\"\n";

    CollectiveInfo_t randCollective = collectives.Random();

    auto MaybeAddBabykickerNote = [&]()
    {
        bool isBabykickerCollective = randCollective == Collective::BabypunchingPuppykickers;
        bool isFirstTime = influences.Get(Collective::BabypunchingPuppykickers) == 1; // Don't get repetitious.
        if (isBabykickerCollective && isFirstTime)
        {
            cout << '\n' << COLLECTIVE_BABYPUNCHING_PUPPYKICKERS_NOTE;
        }
    };

    switch (wish)
    {
    case Wish::Wealth:
        inventory.Add(Item::Gold, 3);
        cout << "You feel your money pouch get a little heavier.";
        break;

    case Wish::Health:
        // There is someone else to grant health to, *and* the genie is feeling silly
        if (!surroundings.IsEmpty() && CoinFlip())
        {
            // Other characters don't actually heal. NPC health is weird in this game.
            cout << "\"You wished for health, you didn't specify for whom\"\nYou notice the "
                 << surroundings.Random() << " appears to have regained some health...";
        }
        // Heal the player
        else
        {
            health.Heal(2);
            cout << "You feel your wounds heal, replenishing 2 health points.";
        }
        break;

    case Wish::Power:
        // Interpret power as political influence
        if (CoinFlip())
        {
            influences.Modify(randCollective, 1);
            cout << "You feel an inexplicable sensation of political influence over the " << randCollective.full << ".";
            MaybeAddBabykickerNote();
        }
        // Interpret power as physical strength
        else
        {
            inventory.Add(Item::Sword, 10);
            cout << "Your blade seems to magically sharpen itself, regaining 10 durability.";
        }
        break;

    case Wish::Status:
        influences.Modify(randCollective, 1);
        cout << "You feel an inexplicable sensation of high status among the " << randCollective.full << ".";
        MaybeAddBabykickerNote();
        break;

    case Wish::Luck:
        luck.Give(Luck::Good, 5);
        cout << "Nothing seems to have changed. You aren't even sure if the wish did anything.";
        break;

    case Wish::Faith:
        influences.Modify(randCollective, 1);
        cout << "You feel an inexplicable sensation of religious influence over the " << randCollective.full << ".";
        MaybeAddBabykickerNote();
        break;
    }

    cout << "\nThe genie disappears.";
}
void Player::DoInteraction_Potion_Ducks()
{
    cout << "A dozen ducks pop into existence around you. They seem lost and disoriented for a moment before waddling northeast towards the nearest pond.";
}
void Player::DoInteraction_Potion_Force()
{
    surroundings.ReRoll();
    cout << "[todo]";
}
void Player::DoInteraction_Potion_Salt()
{
    cout << "Your wounds begin burning and you feel dehydrated.";
}
void Player::DoInteraction_Potion_Ants()
{
    cout << "[todo]";
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
    cout << "You are surprised by the sudden appearance of " << (isvowel(demon[0]) ? "an" : "a") << " " << demon << ". ";
    
    if (demon == "imp")
    {
        health.Damage(2);
        cout << "The imp seems rather annoyed, throwing a fireball at you before de-summoning itself. You take 2 points of damage.";
    }
    else if (demon == "warewolf")
    {
        switch (luck.Check())
        {
        case Luck::Good:
            cout << "The warewolf seems friendly, and you get a chance to play fetch with it before it disappears in a puff of smoke.";
            break;

        default:
            health.Damage(2);
            cout << "The warewolf growls at you and bites your arm before disappearing in a puff of smoke. "
                    "You are protected from most of the attack thanks to your armor, but it still takes away 2 points of your health.";
            break;
        }
    }
    else if (demon == "tentacle")
    {
        health.Damage(1);
        cout << "The tentacle slaps you violently, causing you to spin around in place before it slithers back down into its hole, disappearing from view. "
                "You lost 1 point of health.";
    }
    else if (demon == "vampire")
    {
        switch (luck.Check())
        {
        case Luck::Good:
            inventory.Add(Item::Bread, 3);
            cout << "The vampire seemed confused as to why they were summoned, "
                    "but decides now is as decent a time as any to dispose of the garlic bread people keep throwing at them. "
                    "They " << ChooseRandom({ "plop", "stuff", "drop", "place" }) << " the bread into your hand and promptly transform into a small black bat, "
                    "flying away into the darkness.";
            break;

        case Luck::Neutral:
            cout << "The vampire seems frustrated by your frivolous summoning and kicks you in your armored shin before transforming into a bat and gliding off into the night. "
                    "You take no damage, but it still stings.";
            break;

        case Luck::Bad:
            health.Damage(1);
            cout << "The vampire seems frustrated by your frivolous summoning and kicks you in your armored shin before transforming into a bat and gliding off into the night. "
                    "The kick was surprisingly hard, taking 1 point of your health.";
            break;
        }
    }
}
void Player::DoInteraction_Potion_Fire()
{
    health.Damage(1);
    health.statuses.Apply(StatusEffects::Fire);
    cout << "It hurt quite a bit, taking away 1 point of health.";
}
void Player::DoInteraction_Potion_Explode()
{
    health.Damage(3);
    if (CoinFlip())
    {
        health.statuses.Apply(StatusEffects::Fire);
    }
    cout << "You feel your armor heat up tremendously, practically baking you within it. You feel 3 health points drain from your soul.";
}
void Player::DoInteraction_Potion_Tree()
{
    cout << "You transform into a tree.";
}

const string& Player::GetName() const
{
    return name;
}

void Player::Init()
{
    name = PromptLine("What's your name?");
    health    .Init();
    luck      .Init();
    influences.Init();
    inventory .Init();
}

void Player::Save(ostream& ofs) const
{
    ofs << "name: " << name << '\n';
    health    .Save(ofs);
    luck      .Save(ofs);
    inventory .Save(ofs);
    influences.Save(ofs);
}

void Player::Load(istream& ifs)
{
    ifs.ignore(16, ':').ignore(1, ' '); // Need to explicitly ignore the space as well due to getline
    getline(ifs, name);
    health    .Load(ifs);
    luck      .Load(ifs);
    inventory .Load(ifs);
    influences.Load(ifs);
}

Player player;
