#include "Influences.hpp"

int Influences::Get(const string& targetCollective) const
{
    auto it = influence.find(targetCollective);
    if (it != influence.end())
    {
        return it->second;
    }
    return 0;
}

const map<string, int>& Influences::GetAll() const
{
    return influence;
}

void Influences::Modify(const string& targetCollective, int change)
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

void Influences::ClearAll()
{
    influence.clear();
}

void Influences::Set(const string& targetCollective, int amount)
{
    assert(-10 <= amount && amount <= 10);
    influence.insert_or_assign(targetCollective, amount);
}

bool Influences::Check(const string& targetCollective) const
{
    return DiceCheck(10 + Get(targetCollective), 20);
}

void Influences::Init()
{
    ClearAll();
}

void Influences::Save(ostream& ofs) const
{
    ofs << "influences: " << influence.size() << '\n';
    for (const auto& [collective, amount] : influence)
    {
        ofs << "  " << collective << " - " << amount << '\n';
    }
}

void Influences::Load(istream& ifs)
{
    influence.clear();
    size_t numInfluences;
    ifs.ignore(16, ':') >> numInfluences;
    for (size_t i = 0; i < numInfluences; ++i)
    {
        string collective;
        int amount;
        (ifs >> collective).ignore(3, '-') >> amount;
        influence.emplace(collective, amount);
    }
}
