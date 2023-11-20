#include "utilities.hpp"
#include "Luck.hpp"

bool Luck::ApplyLuck(bool success)
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

LuckType Luck::CheckLuck()
{
    LuckType type = CategorizeLuck(luck);
    switch (type)
    {
    case LuckType::Good: --luck; break;
    case LuckType::Neutral:      break;
    case LuckType::Bad:  ++luck; break;
    }
    return type;
}

LuckType Luck::TestLuck()
{
    return CategorizeLuck(luck--);
}

void Luck::GiveLuck(_In_range_(!=, LuckType::Neutral) LuckType kind, _In_range_(>, 0) int amount)
{
    switch (kind)
    {
    case LuckType::Good: luck += amount; break;
    case LuckType::Neutral:              break;
    case LuckType::Bad:  luck -= amount; break;
    }
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
