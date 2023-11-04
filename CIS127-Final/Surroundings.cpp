#include "Surroundings.hpp"
#include "utilities.hpp"
#include "Collective.hpp"
#include "Interactable.hpp"

Surroundings::~Surroundings()
{
    Clear();
}

void Surroundings::Print() const
{
    cout << "Your current surroundings:\n";
    for (const auto& it : things)
    {
        cout << "- " << it.first << '\n';
    }
}

string Surroundings::Prompt(const string& prompt) const
{
    cout << prompt;
    for (const auto& it : things)
    {
        cout << "\n- " << it.first;
    }
    cout << "\n- self" << endl;
    while (true) // repeats until return
    {
        string input;
        cout << "> ";
        getline(cin, input);
        if (things.contains(input) || input == "self")
        {
            return input;
        }
    }
}

bool Surroundings::Has(const string& shortName) const
{
    return things.contains(shortName);
}

const Interactable& Surroundings::Get(const string& shortName) const
{
    assert(things.contains(shortName));
    return *things.at(shortName);
}

Interactable& Surroundings::Get(const string& shortName)
{
    assert(things.contains(shortName));
    return *things.at(shortName);
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
bool Surroundings::TryAddNew(const string& shortName)
{
    if (things.contains(shortName))
    {
        return false;
    }
    things.emplace(shortName, NewInteractableOfType(shortName));
    return true;
}

// Returns true if removed successfully, otherwise false.
bool Surroundings::TryRemove(const string& shortName)
{
    if (things.contains(shortName))
    {
        delete things.at(shortName);
        things.erase(shortName);
        return true;
    }
    return false;
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
        TryAddNew("monster");
        
        // 50% chance of having 2 monsters instead of 1 (25% total)
        if (DiceCheck(1, 2))
        {
            TryAddNew("monster");
            
            // 50% chance of having 3 monsters instead of 2 (12.5% total)
            if (DiceCheck(1, 2))
            {
                TryAddNew("monster");
            }
        }
    }
    // 66% of rooms without monsters have an NPC
    else if (DiceCheck(2, 3))
    {
        TryAddNew(ChooseRandom({ "baker", "smith", "wizard" }));
    }
    
    // 75% of all rooms have a door
    if (DiceCheck(3, 4))
    {
        TryAddNew("door");
    }
}

string Surroundings::RandomName() const
{
    vector<string> keys;
    keys.reserve(things.size());
    for (const auto& it : things)
    {
        keys.push_back(it.first);
    }
    return ChooseRandom(keys);
}

void Surroundings::Save(ostream& ofs) const
{
    ofs << "surroundings: " << things.size() << '\n';
    for (const auto& [name, thing] : things)
    {
        ofs << "  " << name;
        if (NPC* npc = dynamic_cast<NPC*>(thing))
        {
            ofs << " - " << npc->GetCollective().ShortName();
        }
        else
        {
            ofs << " ;";
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
        string name;
        char separator; // either - or ;
        ifs >> name >> separator;
        if (separator == '-')
        {
            string collective;
            ifs >> collective;
            things.emplace(name, NewNPCOfType(name, Collective::Get(collective)));
        }
        else
        {
            things.emplace(name, NewInteractableOfType(name));
        }
    }
}

Surroundings surroundings;
