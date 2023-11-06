#pragma once
#include <string>
#include <vector>
using namespace std;

enum class Topic
{
    WineFish,
    SkeleStock,
    WoodpeckerSiege,
    NecroFarm,
    Bloodmoon,
    MoleMountain,
    MindflayerRecipe,
    GodFistfight,
    Theseus,
    BlacksmithTeleken,
    NoGarlic,
    PenguinBattle,
    PetMentalHealth,
    Woodchuck,
};

ostream& operator<<(ostream&, Topic);
istream& operator>>(istream&, Topic&);
bool operator==(const string&, Topic);
bool operator==(Topic, const string&);

enum class Item
{
    Bread,  // Heals
    Sword,  // Damages; quantity represents durability
    Potion, // Effects are random
    Gold,   // 100% chance of getting items from NPCs willing to trade (higher chance than bread/talking)
};

ostream& operator<<(ostream&, Item);
istream& operator>>(istream&, Item&);
bool operator==(const string&, Item);
bool operator==(Item, const string&);

Item Prompt(const string& prompt, const vector<Item>& options);

// Sorted from most to least positive effects.
// First half are considered lucky when targeting a friend,
// second half are considered lucky when targeting an enemy
enum class Potion
{
    Predict, // Gives luck, providing an upper hand in anything the player is doing
    Heal,    // Heals the target
    Water,   // Douces the target in water - useless because fire is not DOT and there are no fire elementals
    Wish,    // Gives the target a wish; wish is random unless the target is the player
    Ducks,   // Summons ducks - useless
    Force,   // Pushes the target away - rerolls surroundings if player, removes target from surroundings otherwise
    Salt,    // Random chance to kill a monster
    Ants,    // Summons ants - useless
    Demon,   // Summons a random demon to attack the target
    Fire,    // Deals some damage
    Explode, // Deals high damage; can 1-hit most things including doors
    Tree,    // Turns the target into a tree, soft-locking the game if the target was the player.
    
    // Problem: Softlocking the game is a major enough risk that none of the effects are worth using potions on yourself.
    // Perhaps there could be a chance that NPCs can un-tree the player after one turn? Throwing away a turn is less destructive than a full-on soft-lock.
};

ostream& operator<<(ostream&, Potion);
istream& operator>>(istream&, Potion&);
bool operator==(const string&, Potion);
bool operator==(Potion, const string&);

// Not currently planned for implementation, but not unreasonable.
Potion Prompt(const string& prompt, const vector<Potion>& options);
