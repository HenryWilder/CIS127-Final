#include "Monster.hpp"
#include "Player.hpp"
#include "Surroundings.hpp"
#include <iostream>
using namespace std;

void Monster::DoInteraction_Grab()
{
    bool isPositive = player.influences.Check("monsters");
    
    
    string reaction, reactingTo;
    if (isPositive)
    {
        reaction = ChooseRandom({ "surprised", "impressed", "confused", "baffled", "amused", "astonished", "flustered", "bewildered" });
        reactingTo = ChooseRandom({ "forwardness", "strength", "agility", "fearlessness", "courage" });
    }
    else
    {
        reaction = ChooseRandom({ "surprised", "confused", "unamused", "annoyed", "upset", "angered" });
        reactingTo = ChooseRandom({ "foolishness", "lack of self-preservation", "disrespect" });
    }
    cout << "The monster seems " << reaction << " by your " << reactingTo << ", staring at you until you release it from your grip.\n";
    
    
    if (reaction == "flustered" || ((reaction == "impressed" || reaction == "surprised") && reactingTo == "forwardness"))
    {
        player.inventory.Add("phonenumber");
        cout << "Upon being released, it discretely slips something into your pocket.";
    }
    else if (isPositive)
    {
        player.influences.Modify("monsters", 1);
        cout << "It gives a little nod of respect.";
    }
    else // negative
    {
        player.health.Damage(3);
        cout << "It decides to return the favor, squeezing you in its massive claws as you feel your armor dent and tighten around your fragile, squishy body. ";
        cout << "You lose 3 points of health.";
    }
}
void Monster::DoInteraction_Bread()
{
    bool isPositive = player.influences.Check("monsters");
    
    cout << "The monster ";
    if (isPositive)
    {
        string reaction = ChooseRandom({
            "is delighted by",
            "is overjoyed at",
            "looks longingly at",
            "appriciates",
            "tears up over",
            "stares in surprise at",
            "curiously examines",
            "curiously investigates",
        });
        string response = ChooseRandom({
            "scarfing it down hungrily",
            "taking long, deliberate bites out of it",
            "tucking it away for later",
            "sheathing it on its back like a sword",
            "sliding the entire loaf down its neck in one bite",
            "slipping the loaf into its conveniently-sized hat",
            "stuffing the loaf through what you had previously mistaken for a cycloptic eye",
            "letting the bread melt and assimilate into its smooth, featureless face",
        });
        string gratitude = ChooseRandom({
            "nodding its head with what is meant to be a genuine smile, though it has difficulty trying not to look frightening",
            "giving you what is meant to be a pat on the head; unintentionally causing your helmet to clang against the neck of your breastplate",
            "wrapping its long, heavy arms around you in thanks. It drops you onto the floor after a few seconds, and you lie there stunned for a moment before managing to stand back up",
            "pulling you into a warm bear hug for a full thirty seconds before setting you down",
        });
        player.influences.Modify("monsters", 1);
        cout << reaction << " the gift, quietly accepting it and " << response << " before " << gratitude << ".";
    }
    else
    {
        string reaction = ChooseRandom({ "scowels", "scoffs", "screams", "screeches", "foams", "growls", "hisses", });
        cout << reaction << " at the very idea of accepting a gift from you, slapping it out of your hand and onto the floor.";
    }
}
void Monster::DoInteraction_Sword()
{
    bool isSuccessful = player.luck.Apply(DiceCheck(1, 2));
    if (isSuccessful)
    {
        string bodyCovering = ChooseRandom({ "gooey", "vaporous", "slimey", "meaty", "fuzzy", "fur-covered" });
        cout << "Your sword slashes through the monster, splitting it into two " << bodyCovering << " chunks on the floor. ";
        player.inventory.Add("gold");
        cout << "A gold coin drops to the cold, hard floor with a clink. You pick it up, adding it to your collection as the body fizzles to dust.";
    }
    else
    {
        player.inventory.TryRemove("sword");
        bool isSwordSurviving = player.inventory.Has("sword");
        
        string reaction = ChooseRandom({
            "grabs your sword mid-swing, gripping it tightly in its claws",
            "swats your sword out of your hand, flinging it into the wall",
            "squeezes your sword's blade with two hands",
            "bites down on your sword violently with its massive shark-teeth",
        });
        
        string impact;
        if (isSwordSurviving)
        {
            // adverb
            impact = ChooseRandom({ "slightly", "somewhat", "lightly", "partly", "gently", });
            // verb
            impact = " " + ChooseRandom({ "bending", "dulling", "denting", "melting", "cracking", "chipping", });
            impact += " it";
        }
        else
        {
            // adverb
            impact = ChooseRandom({ "completely", "totally", "fully", "absolutely", "utterly", "instantly", });
            // verb
            impact += " " + ChooseRandom({ "shattering", "obliterating", "liquifying", "anihilating", "evaporating", "disintegrating",
                "digesting", // Yes, even for non-mouth reactions
            });
            impact += " everything above the hilt";
        }
        
        cout << "The monster " << reaction << " and " << impact << '.';
    }
}
void Monster::DoInteraction_Gold()
{
    cout << "The monster doesn't know what to do with the gold, tilting its head curiously but quickly losing interest.";
}

// Talk

void Monster::DoInteraction_Talk_Generic()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_WineFish()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_SkeleStock()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_WoodpeckerSiege()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_NecroFarm()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_Bloodmoon()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_MoleMountain()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_MindflayerRecipe()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_GodFistfight()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_Theseus()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_SmithTeleken()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_NoGarlic()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_PenguinBattle()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_PetMentalHealth()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Talk_Woodchuck()
{
    cout << "[todo]";
}

// Potion

void Monster::DoInteraction_Potion_Predict()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Potion_Heal()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Potion_Water()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Potion_Wish()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Potion_Ducks()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Potion_Force()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Potion_Salt()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Potion_Ants()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Potion_Demon()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Potion_Fire()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Potion_Explode()
{
    cout << "[todo]";
}
void Monster::DoInteraction_Potion_Tree()
{
    cout << "[todo]";
}

string Monster::GetName() const
{
    return "monster";
}
