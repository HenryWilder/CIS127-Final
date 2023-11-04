#include "Luck.hpp"

bool Luck::Apply(bool success)
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
}

int Luck::Check()
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
}

void Luck::Give(int amount)
{
    luck += amount;
}

void Luck::Clear()
{
    luck = 0;
}

void Luck::Set(int amount)
{
    luck = amount;
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
