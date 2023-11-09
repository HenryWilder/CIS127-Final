#include "Surroundings.hpp"
#include "Player.hpp"

Surroundings::~Surroundings()
{
    Clear();
}

void Surroundings::Print() const
{
    ListKeys(cout, things);
}

EntityType Surroundings::Prompt(const string& prompt) const
{
    map options = things;
    options.emplace(EntityType::Player, nullptr);
    return PromptKey(prompt, options);
}

bool Surroundings::Has(EntityType what) const
{
    return things.contains(what);
}

const Entity& Surroundings::Get(EntityType what) const
{
    bool isPlayer = what == EntityType::Player;
    assert(things.contains(what) || isPlayer);
    return isPlayer ? player : *things.at(what);
}

Entity& Surroundings::Get(EntityType what)
{
    bool isPlayer = what == EntityType::Player;
    assert(things.contains(what) || isPlayer);
    return isPlayer ? player : *things.at(what);
}

bool Surroundings::IsEmpty() const
{
    return things.empty();
}

void Surroundings::Clear()
{
    for (auto& thing : things)
    {
        delete thing.second;
    }
    things.clear();
}

// Returns true if added successfully, otherwise false.
bool Surroundings::TryAddNew(EntityType what)
{
    if (!things.contains(what))
    {
        things.emplace(what, NewInteractableOfType(what));
        return true;
    }
    return false;
}

// Returns true if removed successfully, otherwise false.
bool Surroundings::TryRemove(EntityType what)
{
    auto it = things.find(what);
    if (it != things.end())
    {
        delete it->second;
        things.erase(it);
        return true;
    }
    return false;
}

void Surroundings::QueueForRemoval(EntityType what) const
{
    toRemove.push_back(what);
}


// Releases all entities queued for removal. Call when entities are not in use.

void Surroundings::ReleaseQueued()
{
    for (EntityType key : toRemove)
    {
        (void)TryRemove(key);
    }
    toRemove.clear();
}

void Surroundings::ReRoll()
{
    Clear();
    
    const vector<string> possible = {
        "door",    // Do nothing :P
        "baker",   // Give bread
        "smith",   // Repair sword
        "wizard",  // Give potion
        "monster", // Hurt the player
    };
    
    // 50% of rooms have a monster
    if (DiceCheck(1, 2))
    {
        TryAddNew(EntityType::Monster);
        
        // 50% chance of having 2 monsters instead of 1 (25% total)
        if (DiceCheck(1, 2))
        {
            TryAddNew(EntityType::Monster);
            
            // 50% chance of having 3 monsters instead of 2 (12.5% total)
            if (DiceCheck(1, 2))
            {
                TryAddNew(EntityType::Monster);
            }
        }
    }
    // 66% of rooms without monsters have an NPC
    else if (DiceCheck(2, 3))
    {
        TryAddNew(ChooseRandom({ EntityType::Baker, EntityType::Blacksmith, EntityType::Wizard }));
    }
    
    // 75% of all rooms have a door
    if (DiceCheck(3, 4))
    {
        TryAddNew(EntityType::Door);
    }
}

EntityType Surroundings::Random() const
{
    return ChooseRandomKey(things);
}

void Surroundings::Init()
{
    ReRoll();
}

void Surroundings::Save(ostream& ofs) const
{
    ofs << "surroundings: " << things.size() << '\n';
    for (const auto& [name, thing] : things)
    {
        ofs << "  " << name;
        if (NPC* npc = dynamic_cast<NPC*>(thing))
        {
            ofs << " - " << npc->GetCollective();
        }
        ofs << '\n';
    }
}

void Surroundings::Load(istream& ifs)
{
    Clear();
    size_t numThings;
    ifs.ignore(16, ':') >> numThings;
    for (size_t i = 0; i < numThings; ++i)
    {
        EntityType type;
        ifs >> type;
        if (IsNPCType(type))
        {
            Collective collective;
            ifs.ignore(3, '-') >> collective;
            things.emplace(type, NewNPCOfType((NPCType)type, collective));
        }
        else
        {
            things.emplace(type, NewInteractableOfType(type));
        }
    }
}

Surroundings surroundings;
