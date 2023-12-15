#include "utilities/pch/utilities.hpp"
#include "Player.hpp"
#include "helpers/Surroundings.hpp"
#include "player-comps/player-components.hpp"
#include "utilities/randomness.hpp"
#include "utilities/prompt.hpp"
#include "helpers/echo.hpp"

// IMPORTANT:
// All interactions defined in this file are made with the assertion that player is targeting "self".

void Player::DoInteraction_Grab( )
{
    echo << "...Weirdo.\n";
}
void Player::DoInteraction_Bread( )
{
    echo << "It was delicious, replenishing 1 point of health.\n";
    Heal(1);
}
void Player::DoInteraction_Sword( )
{
    switch (TestLuck( ))
    {
        case LuckType::Good:
            echo << "Fortunately, your armor is strong enough and your sword so cumbersome at such an angle that you barely scuffed yourself.\n";
            break;

        case LuckType::Neutral:
            Damage(1);
            echo << "Dummy.\n";
            break;

        case LuckType::Bad:
            Damage(999);
            echo << "What did you think was going to happen?\n";
            break;
    }
}
void Player::DoInteraction_Gold( )
{
    echo << "You make a show of thanking yourself kindly for the gold before returning it to your money pouch.\n";
    AddItem(Item::Gold);
}

// Talk

void NearbyThinksYou(const vector<string> &contextualResponses)
{
    string contextualOpinion = ChooseRandom(
        "may have overlooked",
        "exaggerated",
        "are overestimating",
        "underestimate",
        "may forget",
        "fail to appriciate",
        "are perpetuating harmful stereotypes regarding",
        "make a good point about");

    echo << "The " << surroundings.Random( ) << " thinks you " << contextualOpinion << " " << ChooseRandom(contextualResponses) << ".\n";
}

void Player::DoInteraction_Talk(Topic topic)
{
    if (surroundings.IsEmpty())
    {
        echo << ChooseRandom("Fortunately", "Sadly") << ", nobody was around to hear it...";
        return;
    }

    vector<string> contextualResponses;

    switch (topic)
    {
        case Topic::WineFish:
            contextualResponses = {
                "the resiliance of aquatic livers",
                "the work ethic of drunk merfolk",
                "how that would affect the wine market"
            };
            break;

        case Topic::SkeleStock:
            contextualResponses = {
                "the staying power of good steel",
                "the dwindling numbers of the skeleton army",
                "how important armor is to the health of the Skeleton Alliance's trade sector"
            };
            break;

        case Topic::WoodpeckerSiege:
            contextualResponses = {
                "an average woodpecker's endurance",
                "the fact that a woodpecker pecks wood, not stone",
                "the effectiveness of hot oil on an army of woodpeckers"
            };
            break;

        case Topic::BlacksmithTeleken:
            contextualResponses = {
                "the power of hydrokenesis when used to cool hot steel",
                "the craftsmanship needed to shape tools with one's mind",
                "the quality of a blade heated with telekenetic precision"
            };
            break;

        case Topic::PenguinBattle:
            contextualResponses = {
                "what happened the last six times the Penguin Guild and the Old Realm got into a fight like this in the past",
                "just how much DNA the Penguin Guild's members actually share with real penguins",
                "how prone the Old Realm's shadow government is to shooting themselves in the foot when it comes to matters like this"
            };
            break;

        case Topic::Woodchuck:
            contextualResponses = {
                "where they'd even get all that wood",
                "how long a woodchuck's arms are",
                "the possibility of seven or more woodchucks banding together and forming a woodchuck mega-fusion"
            };
            break;

        case Topic::NecroFarm:
        case Topic::Bloodmoon:
        case Topic::MoleMountain:
        case Topic::MindflayerRecipe:
        case Topic::GodFistfight:
        case Topic::Theseus:
        case Topic::NoGarlic:
        case Topic::PetMentalHealth:
        default:
            contextualResponses = {
                "found it rather odd",
                "thinks you might need a checkup",
                "considers your points rather thought-provoking and insightful, if a little one-sided",
                "is too distracted by your utterly repulsive views to care that you were talking to a stone wall",
                "is confused who you were talking to",
                "looked around to check if there were any hidden familiars you may have been putting on a performance for",
                "gave you a weird look",
                "thinks you underestimate the effectiveness of brute force"
            };
            break;
    }

    NearbyThinksYou(contextualResponses);
}

// Potion

void Player::DoInteraction_Potion(Potion potion)
{
    switch (potion)
    {
        case Potion::Predict:
            echo << "You receive foresight of everything that will happen in the near future, and plan accordingly.\n";
            GiveLuck(LuckType::Good, 5);
            break;

        case Potion::Heal:
            echo << "You felt rejuvenated.\n";
            Heal(1);
            break;

        case Potion::Water:
            echo << "It soothes your wounds. Hopefully this doesn't rust your armor.\n";
            // Todo: status effect
            break;

        case Potion::Wish:
        {
            echo << "A genie appears from the the potion bottle.\n";

            Wish wish = wishes.Prompt("\"What would you like for your wish, " + GetName( ) + "?\"");
            echo << "The genie pauses for a moment before replying, \"Granted.\"\n";

            CollectiveInfo_t randCollective = collectives.Random( );

            auto MaybeAddBabykickerNote = [&]( )
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
                    echo << "You feel your money pouch get a little heavier.\n";
                    AddItem(Item::Gold, 3);
                    break;

                case Wish::Health:
                    // There is someone else to grant health to, *and* the genie is feeling silly
                    if (!surroundings.IsEmpty( ) && CoinFlip( ))
                    {
                        // Other characters don't actually heal. NPC health is weird in this game.
                        echo << "\"You wished for health, you didn't specify for whom\"\nYou notice the "
                            << surroundings.Random( ) << " appears to have regained some health...\n";
                    }
                    // Heal the player
                    else
                    {
                        echo << "You feel your wounds heal, replenishing 2 health points.\n";
                        Heal(2);
                    }
                    break;

                case Wish::Power:
                    // Interpret power as political influence
                    if (CoinFlip( ))
                    {
                        echo << "You feel an inexplicable sensation of political influence over the " << randCollective.full << ".\n";
                        MaybeAddBabykickerNote( );
                        ModifyInfluence(randCollective, +1);
                    }
                    // Interpret power as physical strength
                    else
                    {
                        echo << "Your blade seems to magically sharpen itself, regaining 10 durability.\n";
                        AddItem(Item::Sword, 10);
                    }
                    break;

                case Wish::Status:
                    echo << "You feel an inexplicable sensation of high status among the " << randCollective.full << ".\n";
                    MaybeAddBabykickerNote( );
                    ModifyInfluence(randCollective, +1);
                    break;

                case Wish::Luck:
                    echo << "Nothing seems to have changed. You aren't even sure if the wish did anything.\n";
                    GiveLuck(LuckType::Good, 5);
                    break;

                case Wish::Faith:
                    echo << "You feel an inexplicable sensation of religious influence over the " << randCollective.full << ".\n";
                    MaybeAddBabykickerNote( );
                    ModifyInfluence(randCollective, +1);
                    break;
            }

            echo << "\nThe genie disappears.\n";
        }
            break;

        case Potion::Ducks:
            echo << "A dozen ducks pop into existence around you. They seem lost and disoriented for a moment before waddling northeast towards the nearest pond.\n";
            break;

        case Potion::Force:
            echo << "You are blown through a wall into another room.\n";
            surroundings.ReRoll( );
            break;

        case Potion::Salt:
            echo << "Your wounds begin burning and you feel dehydrated.\n";
            break;

        case Potion::Ants:
            echo << "Ants begin crawling all across your body.\n";
            break;

        case Potion::Demon:
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
                echo << "The imp seems rather annoyed, throwing a fireball at you before de-summoning itself. You take 2 points of damage.\n";
                Damage(2);
            }
            else if (demon == "warewolf")
            {
                switch (CheckLuck( ))
                {
                    case LuckType::Good:
                        echo << "The warewolf seems friendly, and you get a chance to play fetch with it before it disappears in a puff of smoke.\n";
                        break;

                    case LuckType::Neutral:
                    case LuckType::Bad:
                    default:
                        echo << "The warewolf growls at you and bites your arm before disappearing in a puff of smoke. "
                            "You are protected from most of the attack thanks to your armor, but it still does some damage.\n";
                        Damage(2);
                        break;
                }
            }
            else if (demon == "tentacle")
            {
                echo << "The tentacle slaps you violently, causing you to spin around in place before it slithers back down into its hole, disappearing from view.\n";
                Damage(1);
            }
            else if (demon == "vampire")
            {
                switch (CheckLuck( ))
                {
                    case LuckType::Good:
                        echo << "The vampire seemed confused as to why they were summoned, "
                            "but decides now is as decent a time as any to dispose of the garlic bread people keep throwing at them. "
                            "They " << ChooseRandom({ "plop", "stuff", "drop", "place" }) << " the bread into your hand and promptly transform into a small black bat, "
                            "flying away into the darkness.\n";
                        AddItem(Item::Bread, 3);
                        break;

                    case LuckType::Neutral:
                        echo << "The vampire seems frustrated by your frivolous summoning and kicks you in your armored shin before transforming into a bat and gliding off into the night. "
                            "You take no damage, but it still stings.\n";
                        break;

                    case LuckType::Bad:
                        echo << "The vampire seems frustrated by your frivolous summoning and kicks you in your armored shin before transforming into a bat and gliding off into the night. "
                            "The kick was surprisingly hard, taking 1 point of your health.\n";
                        Damage(1);
                        break;
                }
            }
        }
            break;

        case Potion::Fire:
            echo << "It hurt quite a bit.\n";
            Damage(1);
            ApplyStatusEffect(StatusEffects::Fire);
            break;

        case Potion::Explode:
            echo << "You feel your armor heat up tremendously, practically baking you within it.\n";
            Damage(3);
            if (CoinFlip( ))
            {
                ApplyStatusEffect(StatusEffects::Fire);
            }
            break;

        case Potion::Tree:
            echo << "You transform into a tree.\n";
            ApplyStatusEffect(StatusEffects::Tree);
            break;
    }
}

void Player::Init()
{
    Inventory    ::Init( );
    Influences   ::Init( );
    Health       ::Init( );
    StatusEffects::Init( );
    Luck         ::Init( );
}

void Player::Save(ostream &ofs) const
{
    ofs << "name: " << name << '\n';
    Inventory    ::Save(ofs);
    Influences   ::Save(ofs);
    Health       ::Save(ofs);
    StatusEffects::Save(ofs);
    Luck         ::Save(ofs);
}

void Player::Load(istream &ifs)
{
    ifs.ignore(16, ':').ignore(1, ' '); // Need to explicitly ignore the space as well due to getline
    getline(ifs, name);
    Inventory    ::Load(ifs);
    Influences   ::Load(ifs);
    Health       ::Load(ifs);
    StatusEffects::Load(ifs);
    Luck         ::Load(ifs);
}

// Global instance of the player so that other entities can interact with it
Player player;
