#include "Collective.hpp"
#include "utilities.hpp"

map<string, Collective> Collective::allCollectives;

const string& Collective::ShortName() const
{
    return shortName;
}

const string& Collective::FullName() const
{
    return fullName;
}

void Collective::Create(string shortName, string fullName)
{
    allCollectives.insert({ shortName, Collective(shortName, fullName) });
}

const Collective& Collective::Get(const string& shortName)
{
    // This function is not meant to check for the existence of a collective,
    // only to return it.
    assert(allCollectives.contains(shortName));
    return allCollectives.at(shortName);
}

const map<string, Collective>& Collective::GetAll()
{
    return allCollectives;
}

string Collective::RandomShortName()
{
    vector<string> keys;
    keys.reserve(allCollectives.size());
    for (const auto& it : allCollectives)
    {
        keys.push_back(it.first);
    }
    return ChooseRandom(keys);
}

const Collective& Collective::Random()
{
    return Get(RandomShortName());
}
