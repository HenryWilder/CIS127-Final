#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>
using namespace std;

constexpr size_t NUM_COLLECTIVES = 10;

// Interface for classes that can be mutated but not reassigned
// Useful for objects used exclusively as properties
class NotCopyable
{
protected:
    // Default ctor/dtor
    
    constexpr NotCopyable() = default;
    ~NotCopyable() = default;
    
    // Delete assignment operators
    
    NotCopyable(const NotCopyable&) = delete;
    NotCopyable& operator=(const NotCopyable&) = delete;
};

constexpr bool IsExactlyOneFlagSet(size_t flags)
{
    bool isSetBitEncountered = false;
    while (flags)
    {
        if (flags & 1)
        {
            if (isSetBitEncountered)
            {
                return false;
            }
            isSetBitEncountered = true;
        }
        flags >>= 1;
    }
    return isSetBitEncountered;
}

string Prompt(const string& prompt, const vector<string>& options);
string PromptItem(const string& prompt, const map<string, int>& options);

void _RerollSurroundings(vector<string>& surroundings);

class NotImplementedException :
    public exception
{
    const char* what() const noexcept override
    {
        return "feature not implemented";
    }
};

string ChooseRandom(const vector<string>& options);
template<size_t _Size> string ChooseRandom(const array<string, _Size>& options);

bool isvowel(char ch);

class StatusEffects :
    private NotCopyable
{
public:
    // Bitflags
    enum StatusFlags : char
    {
        Fire   = 1, // Cleared by water
        Poison = 2, // Cleared by antidote
        Tree   = 4, // Cleared by [TODO]
    };
    
    // Returns the combined set of all active status flags.
    char Get() const
    {
        return statuses;
    }
    
    // Returns true of all of the provided statuses are active.
    // Multiple statuses are tested at once by combining them with `|`.
    bool HasEvery(StatusFlags testStatuses) const
    {
        return (statuses & testStatuses) == testStatuses;
    }
    
    // Returns true of one or more of the provided statuses are active.
    // Multiple statuses are tested at once by combining them with `|`.
    bool HasAny(StatusFlags testStatuses) const
    {
        return statuses & testStatuses;
    }
    
    // Alias for "HasAnyStatus()" to add clarity when testing only one status.
    bool Has(StatusFlags testStatus) const
    {
        assert(IsExactlyOneFlagSet(testStatus));
        return statuses & testStatus;
    }
    
    // Sets the provided status(es) to true.
    // Multiple statuses can be set at once by combining them with `|`.
    void Apply(StatusFlags applyStatuses)
    {
        statuses |= applyStatuses;
    }
    
    // Sets the provided status(es) to false.
    // Multiple statuses can be cleared at once by combining them with `|`.
    // Default: clears all statuses.
    void Clear(StatusFlags clearStatuses = (StatusFlags)~0)
    {
        statuses &= ~clearStatuses;
    }
    
private:
    char statuses;
};

class Health :
    private NotCopyable
{
public:
    int Get() const
    {
        return health;
    }
    
    int GetMax() const
    {
        return maxHealth;
    }
    
    // Only for use when loading from file.
    void Set(int health, int maxHealth)
    {
        this->health    = health;
        this->maxHealth = maxHealth;
    }
    
    // Returns quantity of health points exceeding the max health (and not added).
    // If all points were added successfully, returns 0.
    int Heal(int points)
    {
        assert(points > 0);
        assert(health <= maxHealth);
        int possibleToHeal = maxHealth - health;
        if (possibleToHeal <= points)
        {
            health += points;
            return 0;
        }
        else
        {
            health = maxHealth;
            return points - possibleToHeal;
        }
    }
    
    // Returns true if survived, returns false if dead.
    bool Damage(int points)
    {
        assert(points > 0);
        if (health >= points)
        {
            health -= points;
        }
        else
        {
            health = 0;
        }
        return health != 0;
    }
    
public:
    StatusEffects statuses;
    
private:
    int health, maxHealth;
};

class Inventory :
    private NotCopyable
{
public:
    // Returns true if the item exists in the inventory; otherwise false.
    bool Has(const string& key) const
    {
        return items.contains(key);
    }
    
    // Returns the quantity of the requested item that exist in the inventory.
    // If the item is not in the inventory, returns 0.
    int Count(const string& key) const
    {
        auto it = items.find(key);
        if (it != items.end())
        {
            return it->second;
        }
        return 0;
    }
    
    // Returns a readonly reference to the inventory map.
    const map<string, int>& GetAll() const
    {
        return items;
    }
    
    // Increases the count of the item by
    void Add(const string& key, int count = 1)
    {
        assert(count > 0);
        if (items.contains(key))
        {
            items.at(key) += count;
        }
        else
        {
            items.emplace(key, count);
        }
    };
    
    // If failed to remove the item (e.g. item is missing or in insufficient quantity), returns -1.
    // When trying to remove a greater quantity of the item than is available, none is removed.
    // If successful, returns how many of that item remain.
    int TryRemove(const string& key, int count = 1)
    {
        assert(count > 0);
        auto it = items.find(key);
        if (it != items.end())
        {
            if (it->second == count)
            {
                items.erase(it);
                return 0;
            }
            else if (it->second > count)
            {
                return it->second -= count;
            }
        }
        return -1;
    };
    
    // Removes the specified quantity of the item. If the item is in insufficient quantity, it is entirely removed.
    // Returns true if the removal would have been successful under normal circumstances,
    // returns false if the quantity of the item was insufficient for the removal (but all were removed anyway).
    bool ForceRemove(const string& key, int count = 1)
    {
        assert(count > 0);
        auto it = items.find(key);
        if (it != items.end())
        {
            if (it->second <= count)
            {
                items.erase(it);
                return it->second == count;
            }
            else // it->second > count
            {
                it->second -= count;
                return true;
            }
        }
        return false;
    }
    
    // Eliminates all instances of the provided item from the inventory no matter the current quantity.
    void RemoveAll(const string& key)
    {
        items.erase(key);
    }
    
    // Eliminates all items from the inventory.
    void RemoveAll()
    {
        items.clear();
    }
    
    // Prints the inventory to the console
    void Print() const
    {
        cout << "[todo]";
    }
    
private:
    map<string, int> items;
};

class Collective :
    private NotCopyable
{
private:
    // Private constructor enforces singleton-like collectives,
    // where there are multiple collectives but each is a singleton of itself.
    Collective(const string& shortName, const string& fullName) :
        shortName(shortName), fullName(fullName) {}
    
public:
    // Returns the shortname (key) associated with the collective.
    const string& ShortName() const
    {
        return shortName;
    }
    
    // Returns the fullname (displayed to the user) of the collective.
    const string& FullName() const
    {
        return fullName;
    }
    
    // Creates a new collective and adds it to the set of known collectives.
    static void Create(string shortName, string fullName)
    {
        allCollectives.emplace(shortName, Collective(shortName, fullName));
    }
    
    // Returns the collective associated with the provided shortname.
    static const Collective& Get(const string& shortName)
    {
        // This function is not meant to check for the existence of a collective,
        // only to return it.
        assert(allCollectives.contains(shortName));
        return allCollectives.at(shortName);
    }
    
    // Returns the iterable collection of all collectives.
    static const map<string, Collective>& GetAll()
    {
        return allCollectives;
    }
    
private:
    const string shortName;
    const string fullName;
    
    static map<string, Collective> allCollectives;
};

// Roll a dice. The roll will succeed on average [chance] times in every [outOf] rolls.
bool DiceCheck(int chance, int outOf)
{
    return (rand() % outOf) < chance;
};

// Encompasses power, faith, and status; [-10..+10]
class Influences :
    private NotCopyable
{
public:
    // Returns the influence associated with the target collective.
    int Get(const string& targetCollective) const
    {
        auto it = influence.find(targetCollective);
        if (it != influence.end())
        {
            return it->second;
        }
        return 0;
    }
    
    // Returns the iterable collection of all influences.
    const map<string, int>& GetAll() const
    {
        return influence;
    }
    
    // Increases/decreases social status among the targeted collective (shortname) by the provided amount.
    // Clamps influence to range [-10..+10]; -50%, 200%, etc. success would be undefined behavior.
    void Modify(const string& targetCollective, int change)
    {
        auto it = influence.find(targetCollective);
        if (it != influence.end())
        {
            it->second = clamp(it->second + change, -10, +10);
        }
        else
        {
            influence.emplace(targetCollective, clamp(change, -10, +10));
        }
    }
    
    // Erases all influence, effectively setting them to 0
    void ClearAll()
    {
        influence.clear();
    }
    
    // Should only be used when loading from a file.
    void Set(const string& targetCollective, int amount)
    {
        assert(-10 <= amount && amount <= 10);
        influence.insert_or_assign(targetCollective, amount);
    }
    
    // Roll a dice for a social check such that:
    // -10 influence gives a   0% chance of success
    //   0 influence gives a  50% chance of success
    // +10 influence gives a 100% chance of success
    bool Check(const string& targetCollective) const
    {
        return DiceCheck(10 + Get(targetCollective), 20);
    }
    
private:
    map<string, int> influence;
};

class Luck :
    private NotCopyable
{
public:
    // If and only if the provided check would have failed naturally, one unit of good luck is consumed
    // to override the failure and replace it with a guaranteed success.
    //
    // If and only if the provided check would have succeeded naturally, one unit of bad luck is consumed
    // to override the success and replace it with a guaranteed failure.
    //
    // If the player has no luck--good nor bad--the success will not influenced and no luck be consumed.
    bool Apply(bool success)
    {
        if (!success && luck > 0)
        {
            --luck;
            return true;
        }
        if (success && luck < 0)
        {
            ++luck;
            return false;
        }
        return success;
    };
    
    // Returns -1, 0, or +1.
    // If the player has good luck, returns +1 and consumes one unit of good luck.
    // If the player has  bad luck, returns -1 and consumes one unit of bad luck.
    // If the player has   no luck, returns  0 and consumes no luck.
    int Check()
    {
        if (luck > 0)
        {
            --luck;
            return 1;
        }
        if (luck < 0)
        {
            ++luck;
            return -1;
        }
        return 0;
    };
    
    // Positive = good luck
    // Negative =  bad luck
    void Give(int amount)
    {
        luck += amount;
    }
    
    // Removes all luck, both good and bad
    void Clear()
    {
        luck = 0;
    }
    
    // Should only be used for loading
    void Set(int amount)
    {
        luck = amount;
    }
    
private:
    int luck = 0;
};

// Abstract base for a non-player thing in the world
// All things have names, alliances, and reactions
class IThing
{
protected:
    virtual void DoInteraction_Talk (const string& topic) = 0;
    virtual void DoInteraction_Grab () = 0;
    virtual void DoInteraction_Bread() = 0;
    virtual void DoInteraction_Sword() = 0;
    virtual void DoInteraction_Gold () = 0;
    
    virtual void DoInteraction_Potion_Predict() = 0;
    virtual void DoInteraction_Potion_Heal   () = 0;
    virtual void DoInteraction_Potion_Water  () = 0;
    virtual void DoInteraction_Potion_Wish   () = 0;
    virtual void DoInteraction_Potion_Ducks  () = 0;
    virtual void DoInteraction_Potion_Force  () = 0;
    virtual void DoInteraction_Potion_Salt   () = 0;
    virtual void DoInteraction_Potion_Ants   () = 0;
    virtual void DoInteraction_Potion_Demon  () = 0;
    virtual void DoInteraction_Potion_Fire   () = 0;
    virtual void DoInteraction_Potion_Explode() = 0;
    virtual void DoInteraction_Potion_Tree   () = 0;
    
    void DoInteraction_Potion(const string& effect)
    {
        if      (effect == "predict") DoInteraction_Potion_Predict();
        else if (effect == "heal"   ) DoInteraction_Potion_Heal   ();
        else if (effect == "water"  ) DoInteraction_Potion_Water  ();
        else if (effect == "wish"   ) DoInteraction_Potion_Wish   ();
        else if (effect == "ducks"  ) DoInteraction_Potion_Ducks  ();
        else if (effect == "force"  ) DoInteraction_Potion_Force  ();
        else if (effect == "salt"   ) DoInteraction_Potion_Salt   ();
        else if (effect == "ants"   ) DoInteraction_Potion_Ants   ();
        else if (effect == "demon"  ) DoInteraction_Potion_Demon  ();
        else if (effect == "fire"   ) DoInteraction_Potion_Fire   ();
        else if (effect == "explode") DoInteraction_Potion_Explode();
        else if (effect == "tree"   ) DoInteraction_Potion_Tree   ();
        else throw new NotImplementedException();
    }
    
public:
    // Returns the shortname for the derived class
    virtual string GetName() const = 0;
    
    // Performs the derived class's reaction to the provided interaction type
    void DoInteraction(const string& action, const string& topicOrEffect)
    {
        if      (action == "talk"  ) DoInteraction_Talk  (topicOrEffect);
        else if (action == "grab"  ) DoInteraction_Grab  ();
        else if (action == "bread" ) DoInteraction_Bread ();
        else if (action == "sword" ) DoInteraction_Sword ();
        else if (action == "gold"  ) DoInteraction_Gold  ();
        else if (action == "potion") DoInteraction_Potion(topicOrEffect);
        else throw new NotImplementedException();
    }
};

class Player :
    private IThing
{
public:
    string GetName() const override
    {
        return name;
    }
    
    void SetName(const string& newName)
    {
        name = newName;
    }
    
public: // Properties
    Health health;
    Inventory inventory;
    Influences influences;
    Luck luck;
    
private:
    string name = "";
};

class Surroundings
{
public:
    // Prints a list of the surroundings to the console
    void Print() const
    {
        cout << "[todo]";
    }
    
    bool Has(const string& shortName) const
    {
        return things.contains(shortName);
    }
    
    const IThing& Get(const string& shortName) const
    {
        assert(things.contains(shortName));
        return things.at(shortName);
    }
    
    IThing& Get(const string& shortName)
    {
        assert(things.contains(shortName));
        return things.at(shortName);
    }
    
    void Clear()
    {
        things.clear();
    }
    
    // Returns true if added successfully, otherwise false.
    bool TryAdd(const string& shortName, IThing& thing)
    {
        if (things.contains(shortName))
        {
            return false;
        }
        things.emplace(shortName, thing);
        return true;
    }
    
    // Returns true if removed successfully, otherwise false.
    bool TryRemove(const string& shortName)
    {
        if (!things.contains(shortName))
        {
            things.erase(shortName);
            return true;
        }
        return false;
    }
    
    void ReRoll()
    {
        Clear();
        // todo
    }
    
private:
    map<string, IThing&> things;
};

void Load(const char* filename, Player& player, Surroundings& surroundings)
{
    
}

void Save(const char* filename, const Player& player, const Surroundings& surroundings)
{
    
}

int main()
{
    const string TOPIC_WINEFISH      = "the effects of water-wine alchemy on the local fish population";
    const string TOPIC_SKELESTOCK    = "the volatile stock price of enchanted skeleton armor";
    const string TOPIC_WP_SIEGE      = "how many woodpeckers it would take to breach the castle wall";
    const string TOPIC_NECROFARM     = "the ethicacy of using necromancy in farming-related fields";
    const string TOPIC_BLOODMOON     = "when the next blood moon may occur";
    const string TOPIC_MOLE_MNT      = "whether enough moles could in fact make a mountain out of their hill";
    const string TOPIC_MF_RECIPE     = "what recipes to use for preparing mind flayer";
    const string TOPIC_GOD_FISTFIGHT = "which gods could win in a fistfight against each other";
    const string TOPIC_THESEUS       = "why Theseus has been getting so fussy about their ship lately";
    const string TOPIC_BS_TELEKEN    = "what form of telekenesis would be the most effective for a blacksmith to use";
    const string TOPIC_NO_GARLIC     = "how the elder wizards should handle the recent garlic & holy water defecits";
    const string TOPIC_PENGUIN_BTL   = "whether the Old Realm needs more supplies or troops to survive their war of attrition against the Penguin Guild";
    const string TOPIC_PET_MNTL_HP   = "the effects of mind-altering spells on the mental health of familiars";
    const string TOPIC_WOODCHUCK     = "the quantity of wood throwable by a woodchuck in a hypothetical scenario that such a feat was possible for the creature";
    
    const array<string, 14> topics = {
        TOPIC_WINEFISH,
        TOPIC_SKELESTOCK,
        TOPIC_WP_SIEGE,
        TOPIC_NECROFARM,
        TOPIC_BLOODMOON,
        TOPIC_MOLE_MNT,
        TOPIC_MF_RECIPE,
        TOPIC_GOD_FISTFIGHT,
        TOPIC_THESEUS,
        TOPIC_BS_TELEKEN,
        TOPIC_NO_GARLIC,
        TOPIC_PENGUIN_BTL,
        TOPIC_PET_MNTL_HP,
        TOPIC_WOODCHUCK,
    };

    const string ITEM_BREAD  = "bread";  // Heals
    const string ITEM_SWORD  = "sword";  // Damages; quantity represents durability
    const string ITEM_POTION = "potion"; // Effects are random
    const string ITEM_GOLD   = "gold";   // 100% chance of getting items from NPCs willing to trade (higher chance than bread/talking)
    
    const array<string, 4> itemNames = {
        ITEM_BREAD,
        ITEM_SWORD,
        ITEM_POTION,
        ITEM_GOLD,
    };
    
    const string POTION_EFFECT_PREDICT = "predict"; // Gives luck, providing an upper hand in anything the player is doing
    const string POTION_EFFECT_HEAL    = "heal";    // Heals the target
    const string POTION_EFFECT_WATER   = "water";   // Douces the target in water - useless because fire is not DOT and there are no fire elementals
    const string POTION_EFFECT_WISH    = "wish";    // Gives the target a wish; wish is random unless the target is the player
    const string POTION_EFFECT_DUCKS   = "ducks";   // Summons ducks - useless
    const string POTION_EFFECT_FORCE   = "force";   // Pushes the target away - rerolls surroundings if player, removes target from surroundings otherwise
    const string POTION_EFFECT_SALT    = "salt";    // Random chance to kill a monster
    const string POTION_EFFECT_ANTS    = "ants";    // Summons ants - useless
    const string POTION_EFFECT_DEMON   = "demon";   // Summons a random demon to attack the target
    const string POTION_EFFECT_FIRE    = "fire";    // Deals some damage
    const string POTION_EFFECT_EXPLODE = "explode"; // Deals high damage; can 1-hit most things including doors
    const string POTION_EFFECT_TREE    = "tree";    // Turns the target into a tree, soft-locking the game if the target was the player.
    
    // Problem: Softlocking the game is a major enough risk that none of the effects are worth using potions on yourself.
    // Perhaps there could be a chance that NPCs can un-tree the player after one turn? Throwing away a turn is less destructive than a full-on soft-lock.

    // Sorted from most to least positive effects.
    // First half are considered lucky when targeting a friend,
    // second half are considered lucky when targeting an enemy
    const array<string, 12> potionEffects = {
        POTION_EFFECT_PREDICT,
        POTION_EFFECT_HEAL,
        POTION_EFFECT_WATER,
        POTION_EFFECT_WISH,
        POTION_EFFECT_DUCKS,
        POTION_EFFECT_FORCE,
        POTION_EFFECT_SALT,
        POTION_EFFECT_ANTS,
        POTION_EFFECT_DEMON,
        POTION_EFFECT_FIRE,
        POTION_EFFECT_EXPLODE,
        POTION_EFFECT_TREE,
    };
    
    // Any given NPC has a random chance of being in a particular collective.
    Collective::Create("western_expance",           "citizens of the Western Expanse");
    Collective::Create("twisting_vacancy",          "Valley of the Twisting Vacancy");
    Collective::Create("infinite_pathways",         "Children of the Pond of Infinite Pathways");
    Collective::Create("northern_seed",             "Followers of the Northern Seed");
    Collective::Create("eastern_swampfire",         "Earthen Swampfire district of the Northeastern Bishop's Domain");
    Collective::Create("critical_malstrom",         "Beholders of the Critical Malstrom");
    Collective::Create("elder_kings",               "Nursing Home of the Elder Kings");
    Collective::Create("dead_king_citadel",         "Holdout Clan of the Dead King's Fallen Citadel");
    Collective::Create("babypunching_puppykickers", "League of Babypunching Puppykickers.");
    Collective::Create("monsters",                  "monsters");
    
    const string COLLECTIVE_BABYPUNCHING_PUPPYKICKERS_NOTE = "You feel a bit of pity for the group's unfortunate name, a poor translation from the fennecborns' native tongue for \"soft-handed littlepaw-walkers\". Maybe with your newfound sway, you can convince them to change their name";
    
    constexpr const char filename[] = "savegame.txt";
    
    Player player;
    vector<string> surroundings;
    
    auto RerollSurroundings = [&surroundings]()
    {
        _RerollSurroundings(surroundings);
    };
    
    bool isResetting = true;
    
    while (isResetting)
    {
        isResetting = false;
        
        // Load the game
        {
            ifstream ifs(filename);
            if (ifs.is_open())
            {
                string pick = Prompt("Start", { "load", "new" });
                
                if (pick == "new")
                {
                    // causes ifs.is_open() to fail
                    ifs.close();
                }
            }
            else
            {
                cout << "No save found. Starting a new game.\n\n";
            }
            
            if (ifs.is_open()) // load game
            {
                size_t numItems, numSurroundings;
                getline(ifs, playerName) >> health >> luck >> isATree >> numItems >> numSurroundings;
                
                for (int& amnt : influence)
                {
                    ifs >> amnt;
                }
                
                for (size_t i = 0; i < numItems; ++i)
                {
                    string itemName;
                    int itemCount;
                    ifs >> itemName >> itemCount;
                    items.emplace(itemName, itemCount);
                }
                
                surroundings.reserve(numSurroundings);
                for (size_t i = 0; i < numSurroundings; ++i)
                {
                    string thing;
                    ifs >> thing;
                    surroundings.push_back(thing);
                }
                
                ifs.close();
                
                cout << "Welcome back, " << playerName;
            }
            else // new game
            {
                cout << "What is your name?";
                playerName = "";
                while (playerName == "")
                {
                    cout << "\n> ";
                    getline(cin, playerName);
                }
                health = 3;
                luck = 0;
                isATree = false;
                influence.fill(0);
                items = { { "gold", 5 }, { "sword", 20 } };
                RerollSurroundings();
            }
        }
        
        cout << endl;
        
        // Game loop
        while (true)
        {
            // Echo surroundings
            cout << "Your current surroundings:\n";
            for (const string& thing : surroundings)
            {
                cout << "- " << thing << '\n';
            }
            cout << "Your current inventory:\n";
            for (const auto& item : items)
            {
                cout << "- " << item.second << " " << item.first << "\n";
            }
            cout << endl;
            
            vector<string> commandOptions;
            if (isATree)
            {
                commandOptions = { "quit", "restart" };
            }
            else
            {
                commandOptions = { "move", "use", "quit", "restart" };
            }
            
            string cmd = Prompt("What would you like to do?", commandOptions);
            
            if (cmd == "move")
            {
                (void)Prompt("Where would you like to move?", { "left", "right", "forward" });
                RerollSurroundings(); // choice is an illusion :P
            }
            else if (cmd == "use")
            {
                // Request
                string action = Prompt("What would you like to use?", { "talk", "grab", "item" });
                
                if (action == "item")
                {
                    if (!items.empty())
                    {
                        action = PromptItem("Which item?", items);
                        TryRemoveItem(action, 1);
                    }
                    else
                    {
                        cout << "You don't have any items.\n";
                        continue;
                    }
                }
                
                if (action == "phonenumber")
                {
                    cout << "If only you had a phone...\n\n";
                    continue;
                }
                
                vector<string> availableTargets = surroundings;
                availableTargets.insert(availableTargets.begin() + 0, "self");
                string target = Prompt("On what?", availableTargets);
                
                string topic_or_effect;
                if (action == "talk")
                {
                    topic_or_effect = ChooseRandom(topics);
                }
                else if (action == "potion")
                {
                    if (luck > 0)
                    {
                        --luck;
                        int potionEffectsHalfSize = potionEffects.size() / 2;
                        if (target == "self") // Lower half
                        {
                            topic_or_effect = potionEffects.at(rand() % potionEffectsHalfSize);
                        }
                        else // Upper half
                        {
                            topic_or_effect = potionEffects.at(potionEffectsHalfSize + (rand() % (potionEffects.size() - potionEffectsHalfSize)));
                        }
                    }
                    else
                    {
                        topic_or_effect = ChooseRandom(potionEffects);
                    }
                }
                
                // Echo
                {
                    cout << "You ";
                    if (action == "talk")
                    {
                        cout << "had an interesting discussion regarding " << topic_or_effect << " with";
                    }
                    else if (action == "grab")
                    {
                        cout << "grabbed";
                    }
                    else if (action == "bread")
                    {
                        cout << "gave a piece of bread to";
                    }
                    else if (action == "sword")
                    {
                        cout << "swung your sword at";
                    }
                    else if (action == "potion")
                    {
                        cout << "used a potion of " << topic_or_effect << " on";
                    }
                    else if (action == "gold")
                    {
                        cout << "gave some gold to";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                    
                    cout << " ";
                    
                    if (target == "self")
                    {
                        cout << "yourself";
                    }
                    else // npc
                    {
                        cout << "the " << target;
                    }
                    cout << ".\n";
                }
                
                // Responses
                if (target == "self")
                {
                    if (action == "talk")
                    {
                        if (!surroundings.empty())
                        {
                            const array<string, 10> possibleResponses = {
                                "found it rather odd",
                                "thinks you might need a checkup",
                                "considers your points rather thought-provoking and insightful, if a little one-sided",
                                "is too distracted by your utterly repulsive views to care that you were talking to a stone wall",
                                "is confused who you were talking to",
                                "looked around to check if there were any hidden familiars you may have been putting on a performance for",
                                "gave you a weird look",
                                "thinks you underestimate the effectiveness of brute force",
                                "thinks you ",
                            };
                            
                            // It is by design that the responder might be inanimate. I thought it would be funny.
                            string responder = ChooseRandom(surroundings);
                            string response = ChooseRandom(possibleResponses);
                            if (response == "thinks you ")
                            {
                                array<string, 7> contextualOpinion = {
                                    "may have overlooked",
                                    "exaggerated",
                                    "are overestimating",
                                    "underestimate",
                                    "may forget",
                                    "fail to appriciate",
                                    "are perpetuating harmful propaganda regarding",
                                };
                                response += ChooseRandom(contextualOpinion) + ' ';
                                array<string, 3> contextualResponses;
                                if (topic_or_effect == TOPIC_WINEFISH)
                                {
                                    contextualResponses = {
                                        "the resiliance of aquatic livers",
                                        "the work ethic of drunk merfolk",
                                        "how that would affect the wine market",
                                    };
                                }
                                else if (topic_or_effect == TOPIC_SKELESTOCK)
                                {
                                    contextualResponses = {
                                        "the staying power of good steel",
                                        "the dwindling numbers of the skeleton army",
                                        "how important armor is to the health of the Skeleton Alliance's trade sector",
                                    };
                                }
                                else if (topic_or_effect == TOPIC_WP_SIEGE)
                                {
                                    contextualResponses = {
                                        "an average woodpecker's endurance",
                                        "the fact that a woodpecker pecks wood, not stone",
                                        "the effectiveness of hot oil on an army of woodpeckers",
                                    };
                                }
                                else if (topic_or_effect == TOPIC_BS_TELEKEN)
                                {
                                    contextualResponses = {
                                        "the power of hydrokenesis when used to cool hot steel",
                                        "the craftsmanship needed to shape tools with one's mind",
                                        "the quality of a blade heated with telekenetic precision",
                                    };
                                }
                                else if (topic_or_effect == TOPIC_WOODCHUCK)
                                {
                                    contextualResponses = {
                                        "where they'd even get all that wood",
                                        "how long a woodchuck's arms are",
                                        "the possibility of seven or more woodchucks banding together and forming a woodchuck mega-fusion",
                                    };
                                }
                                else if (topic_or_effect == TOPIC_PENGUIN_BTL)
                                {
                                    contextualResponses = {
                                        "what happened the last six times the Penguin Guild and the Old Realm got into a fight like this in the past",
                                        "just how much DNA the Penguin Guild's members actually share with real penguins",
                                        "how prone the Old Realm's shadow government is to shooting themselves in the foot when it comes to matters like this",
                                    };
                                }
                                else // make up some catch-all bs
                                {
                                    contextualResponses = {
                                        "how much salt that would take",
                                        "the pulsating, glowing red mass",
                                        "the power of teamwork",
                                    };
                                }
                                response += ChooseRandom(contextualResponses);
                            }
                            cout << "The " << responder << " " << response << ".";
                        }
                        else
                        {
                            cout << (rand() & 1 ? "Fortunately" : "Sadly") << ", nobody was around to hear it...";
                        }
                    }
                    else if (action == "grab")
                    {
                        cout << "Weirdo.";
                    }
                    else if (action == "bread")
                    {
                        ++health;
                        cout << "It was delicious, replenishing 1 point of health.";
                    }
                    else if (action == "sword")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "potion")
                    {
                        if (topic_or_effect == "fire")
                        {
                            --health;
                            cout << "It hurt quite a bit, taking away 1 point of health.";
                        }
                        else if (topic_or_effect == "heal")
                        {
                            ++health;
                            cout << "You felt rejuvenated, recovering 1 point of health.";
                        }
                        else if (topic_or_effect == "water")
                        {
                            cout << "It soothes your wounds. Hopefully this doesn't rust your armor.";
                        }
                        else if (topic_or_effect == "ducks")
                        {
                            cout << "A dozen ducks pop into existence around you. They seem lost and disoriented for a moment before waddling northeast towards the nearest pond.";
                        }
                        else if (topic_or_effect == "explode")
                        {
                            health -= 3;
                            cout << "You feel your armor heat up tremendously, practically baking you within it. You feel 3 health points drain from your soul.";
                        }
                        else if (topic_or_effect == "wish")
                        {
                            cout << "A genie appears from the the potion bottle.";
                            
                            string wish = Prompt("\"What would you like for your wish, " + playerName + "?\"", {
                                "wealth", "health", "power", "status", "luck", "faith"
                            });
                            cout << "The genie pauses for a moment before replying, \"Granted.\"\n";
                            
                            if (wish == "wealth")
                            {
                                if (items.contains("gold"))
                                {
                                    items.at("gold") += 3;
                                }
                                else
                                {
                                    items.emplace("gold", 3);
                                }
                                cout << "You feel your money pouch get a little heavier.";
                            }
                            else if (wish == "health")
                            {
                                if (!surroundings.empty() && (rand() & 1))
                                {
                                    // They didn't :P
                                    cout << "\"You wished for health, you didn't specify for whom\"\nYou notice the "
                                        << ChooseRandom(surroundings) << " appears to have regained some health...";
                                }
                                else
                                {
                                    health += 2;
                                    cout << "You feel your wounds heal, replenishing 2 health points.";
                                }
                            }
                            else if (wish == "power")
                            {
                                if ((rand() & 1))
                                {
                                    size_t collectiveIndex = rand() % collectives.size();
                                    string among = collectives.at(collectiveIndex);
                                    ++influence.at(collectiveIndex);
                                    cout << "You feel an inexplicable sensation of political influence over the " << among << ".";
                                }
                                else
                                {
                                    if (items.contains("sword"))
                                    {
                                        items.at("sword") += 10;
                                    }
                                    else
                                    {
                                        items.emplace("sword", 10);
                                    }
                                    cout << "Your blade seems to magically sharpen itself, regaining 10 durability.";
                                }
                            }
                            else if (wish == "status")
                            {
                                size_t collectiveIndex = rand() % collectives.size();
                                string among = collectives.at(collectiveIndex);
                                ++influence.at(collectiveIndex);
                                cout << "You feel an inexplicable sensation of high status among the " << among << ".";
                            }
                            else if (wish == "luck")
                            {
                                luck += 5;
                                cout << "Nothing seems to have changed. You aren't even sure if the wish did anything.";
                            }
                            else if (wish == "faith")
                            {
                                size_t collectiveIndex = rand() % collectives.size();
                                string among = collectives.at(collectiveIndex);
                                ++influence.at(collectiveIndex);
                                cout << "You feel an inexplicable sensation of religious influence over the " << among << ".";
                            }
                            cout << " The genie disappears.";
                        }
                        else if (topic_or_effect == "demon")
                        {
                            array<string, 4> demonTypes = {
                                "imp",
                                "warewolf",
                                "tentacle",
                                "vampire",
                            };
                            string demon = ChooseRandom(demonTypes);
                            cout << "You are surprised by the sudden appearance of " << (isvowel(demon[0]) ? "an" : "a") << " " << demon << ". ";
                            
                            if (demon == "imp")
                            {
                                health -= 2;
                                cout << "The imp seems rather annoyed, throwing a fireball at you before de-summoning itself. You take 2 points of damage.";
                            }
                            else if (demon == "warewolf")
                            {
                                if (luck > 0)
                                {
                                    --luck;
                                    cout << "The warewolf seems friendly, and you get a chance to play fetch with it before it disappears in a puff of smoke.";
                                }
                                else
                                {
                                    health -= 2;
                                    cout << "The warewolf growls at you and bites your arm before disappearing in a puff of smoke. You are protected from most of the attack thanks to your armor, but it still takes away 2 points of your health.";
                                }
                            }
                            else if (demon == "tentacle")
                            {
                                --health;
                                cout << "The tentacle slaps you violently, causing you to spin around in place before it slithers back down into its hole, disappearing from view. You lost 1 point of health.";
                            }
                            else if (demon == "vampire")
                            {
                                if (luck > 0)
                                {
                                    --luck;
                                    AddItem("bread", 3);
                                    cout << "The vampire seemed confused as to why they were summoned, but decides now is as decent a time as any to dispose of the garlic bread people keep throwing at them. They " << ChooseRandom({ "plop", "stuff", "drop", "place" }) << " it down into your hand and promptly transform into a small black bat, flying away into the darkness.";
                                }
                                else
                                {
                                    cout << "The vampire seems frustrated by your frivolous summoning and kicks you in your armored shin before transforming into a bat and gliding off into the night. You take no damage, but it still stings.";
                                }
                            }
                        }
                        else if (topic_or_effect == "salt")
                        {
                            cout << "Your wounds begin burning and you feel dehydrated.";
                        }
                        else if (topic_or_effect == "tree")
                        {
                            cout << "You transform into a tree.";
                        }
                        else if (topic_or_effect == "ants")
                        {
                            cout << "[TODO]\n";
                        }
                        else if (topic_or_effect == "force")
                        {
                            cout << "[TODO]\n";
                        }
                        else if (topic_or_effect == "predict")
                        {
                            luck += 5;
                            cout << "You recieve foresight of everything that will happen in the near future, and plan accordingly.";
                        }
                    }
                    else if (action == "gold")
                    {
                        AddItem("gold", 1);
                        cout << "You make a show of thanking yourself kindly for the gold before returning it to your money pouch.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else if (target == "door")
                {
                    if (action == "talk")
                    {
                        cout << "The door listens silently.";
                    }
                    else if (action == "grab")
                    {
                        RerollSurroundings();
                        cout << "The door opens, allowing you through.";
                    }
                    else if (action == "bread")
                    {
                        ++luck;
                        cout << "The door says nothing, but you get the odd sensation that it appriciates the gift.";
                    }
                    else if (action == "sword")
                    {
                        if (rand() & 3) // 3/4 chance - 01, 10, 11 = true; 00 = false
                        {
                            cout << "The door does not yield.";
                        }
                        else
                        {
                            RerollSurroundings();
                            cout << "The door crumbles to bits and you walk through it.";
                        }
                    }
                    else if (action == "potion")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "gold")
                    {
                        cout << "The door remains silent and unmoved. It has no concept for money.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else if (target == "baker")
                {
                    if (action == "talk")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "grab")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "bread")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "sword")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "potion")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "gold")
                    {
                        AddItem("bread", 1);
                        cout << "The baker thanks you for the gold and hands you a piece of bread.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else if (target == "smith")
                {
                    if (action == "talk")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "grab")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "bread")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "sword")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "potion")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "gold")
                    {
                        AddItem("sword", 10);
                        cout << "The blacksmith thanks you for the gold and adds some durability to your sword.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else if (target == "wizard")
                {
                    if (action == "talk")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "grab")
                    {
                        bool isPositive = rand() & 1;
                        string reaction, actionDescriptor;
                        if (isPositive)
                        {
                            reaction = ChooseRandom({ "intrigued", "embarrassed" });
                            actionDescriptor = ChooseRandom({ "surprised", "unamused", "annoyed", "" });
                        }
                        else
                        {
                            reaction = ChooseRandom({ "surprised", "unamused", "annoyed", "" });
                            actionDescriptor = ChooseRandom({ "surprised", "unamused", "annoyed", "" });
                        }
                        cout << "The wizard appears " << reaction << " by your " << actionDescriptor << " action.";
                    }
                    else if (action == "bread")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "sword")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "potion")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "gold")
                    {
                        AddItem("potion", 1);
                        cout << "The wizard thanks you for the gold and gives you a potion.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else if (target == "monster")
                {
                    bool isPositive = (rand() % 20) < influence.at(MONSTER_COLLECTIVE);
                    
                    // Only uses luck if you wouldn't have succeeded and weren't trying to hurt it
                    if (!isPositive && luck > 0 && action != "sword" && action != "potion")
                    {
                        --luck;
                        isPositive = true;
                    }
                    
                    if (action == "talk")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "grab")
                    {
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
                            AddItem("phonenumber", 1);
                            cout << "Upon being released, it discretely slips something into your pocket.";
                        }
                        else if (isPositive)
                        {
                            ++influence.at(MONSTER_COLLECTIVE);
                            cout << "It gives a little nod of respect.";
                        }
                        else // negative
                        {
                            health -= 3;
                            cout << "It decides to return the favor, squeezing you in its massive claws as you feel your armor dent and tighten around your fragile, squishy body. ";
                            cout << "You lose 3 points of health.";
                        }
                    }
                    else if (action == "bread")
                    {
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
                            ++influence.at(MONSTER_COLLECTIVE);
                            cout << reaction << " the gift, quietly accepting it and " << response << " before " << gratitude << ".";
                        }
                        else
                        {
                            string reaction = ChooseRandom({ "scowels", "scoffs", "screams", "screeches", "foams", "growls", "hisses", });
                            cout << reaction << " at the very idea of accepting a gift from you, slapping it out of your hand and onto the floor.";
                        }
                    }
                    else if (action == "sword")
                    {
                        bool isSuccessful = rand() & 1; // 50% chance
                        if (!isSuccessful && luck > 0)
                        {
                            --luck;
                            isSuccessful = true;
                        }
                        
                        if (isSuccessful)
                        {
                            string bodyCovering = ChooseRandom({ "gooey", "vaporous", "slimey", "meaty", "fuzzy", "fur-covered" });
                            cout << "Your sword slashes through the monster, splitting it into two " << bodyCovering << " chunks on the floor. ";
                            AddItem("gold", 1);
                            cout << "A gold coin drops to the cold, hard floor with a clink. You pick it up, adding it to your collection as the body fizzles to dust.";
                        }
                        else
                        {
                            TryRemoveItem("sword", 1);
                            bool isSwordSurviving = items.contains("sword");
                            
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
                    else if (action == "potion")
                    {
                        cout << "[TODO]\n";
                    }
                    else if (action == "gold")
                    {
                        cout << "The monster doesn't know what to do with the gold, tilting its head curiously but quickly losing interest.";
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
                else
                {
                    throw new NotImplementedException();
                }
                
                cout << endl;
            }
            else if (cmd == "quit")
            {
                isResetting = false;
                break;
            }
            else if (cmd == "restart")
            {
                isResetting = true;
                break;
            }
            
            cout << endl;
            
            if (health <= 0)
            {
                cout << "Your health has dropped to zero and you have died.\n";
                string tryAgain = Prompt("Would you like to start again?", { "yes", "no" });
                if (tryAgain == "yes")
                {
                    isResetting = true;
                    break;
                }
            }
        }
    }
    
    // Save the game
    {
        ofstream ofs(filename, ofstream::trunc);
        ofs << playerName << '\n';
        ofs << health << ' ' << luck << ' ' << isATree << ' ' << items.size() << ' ' << surroundings.size();
        
        for (int& amnt : influence)
        {
            ofs << ' ' << amnt;
        }
        ofs << '\n';
        
        for (const auto& item : items)
        {
            ofs << item.first << ' ' << item.second << '\n';
        }
        
        for (const string& thing : surroundings)
        {
            ofs << thing << '\n';
        }
        
        ofs.close();
    }
    
    return 0;
}

string Prompt(const string& prompt, const vector<string>& options)
{
    cout << prompt;
    for (const string& opt : options)
    {
        cout << "\n- " << opt;
    }
    cout << endl;
    while (true) // repeats until return
    {
        string input;
        cout << "> ";
        getline(cin, input);
        auto it = find(options.begin(), options.end(), input);
        if (it != options.end())
        {
            return *it;
        }
    }
}

string PromptItem(const string& prompt, const map<string, int>& options)
{
    cout << prompt;
    for (const auto& it : options)
    {
        cout << "\n- " << it.first << " : " << it.second;
    }
    cout << endl;
    while (true) // repeats until return
    {
        string input;
        cout << "> ";
        getline(cin, input);
        auto it = options.find(input);
        if (it != options.end())
        {
            return it->first;
        }
    }
}

string ChooseRandom(const vector<string>& options)
{
    return options.at(rand() % options.size());
}

template<size_t _Size>
string ChooseRandom(const array<string, _Size>& options)
{
    return options.at(rand() % options.size());
}

void _RerollSurroundings(vector<string>& surroundings)
{
    surroundings.clear();
    
    const array<string, 5> possible = {
        "door",    // Do nothing :P
        "baker",   // Give bread
        "smith",   // Repair sword
        "wizard",  // Give potion
        "monster", // Hurt the player
    };
    
    for (int i = 0; i < 4; ++i) // Four possible items per roll
    {
        if (rand() & 1) // Whether to *try* adding something to surroundings (not guaranteed)
        {
            string choice = ChooseRandom(possible);
            
            // Duplicates are not added
            if (find(surroundings.begin(), surroundings.end(), choice) == surroundings.end())
            {
                surroundings.push_back(choice);
            }
        }
    }
}

bool isvowel(char ch)
{
    if (isalpha(ch))
    {
        if (islower(ch))
        {
            return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u';
        }
        else
        {
            return ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U';
        }
    }
    return false;
}
