#include "Luck.hpp"

bool Luck::Apply(bool success)
{
    if (!success || luck > 0)
    {
        --luck;
        return true;
    }
    else if (success && luck < 0)
    {
        ++luck;
        return false;
    }
    return success;
}

Luck::LuckType Luck::Check()
{
    LuckType type = CategorizeLuck(luck);
    switch (type)
    {
    case Good: --luck; break;
    case Bad:  ++luck; break;
    }
    return type;
}

Luck::LuckType Luck::Test()
{
    return CategorizeLuck(luck--);
}

void Luck::Give(_In_range_(!=, Neutral) LuckType kind, int amount)
{
    switch (kind)
    {
    case Luck::Good: luck += amount; break;
    case Luck::Bad:  luck -= amount; break;
    }
}

void Luck::Clear()
{
    luck = 0;
}

void Luck::Init()
{
    luck = 0;
}

void Luck::Save(ostream& ofs) const
{
    ofs << "luck: " << luck << '\n';
}

void Luck::Load(istream& ifs)
{
    ifs.ignore(16, ':') >> luck;
}
