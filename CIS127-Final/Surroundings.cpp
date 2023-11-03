#include "Surroundings.hpp"
#include "utilities.hpp"
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
    for (auto thing : things)
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
    things.emplace(shortName, NewThingOfType(shortName));
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
    
    for (int i = 0; i < 4; ++i) // Four MAXIMUM possible items per roll
    {
        if (DiceCheck(1, 2)) // Whether to *try* adding something to surroundings (not guaranteed)
        {
            TryAddNew(ChooseRandom(possible));
        }
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

Surroundings surroundings;
