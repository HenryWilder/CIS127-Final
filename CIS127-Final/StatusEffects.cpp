#include "utilities.hpp"
#include "StatusEffects.hpp"

bool StatusEffects::HasEveryStatusEffect(StatusFlags testStatuses) const
{
    return (statuses & testStatuses) == testStatuses;
}

bool StatusEffects::HasAnyStatusEffect(StatusFlags testStatuses) const
{
    return statuses & testStatuses;
}

bool StatusEffects::HasStatusEffect(StatusFlags testStatus) const
{
    assert(IsExactlyOneBitSet(testStatus));
    return statuses & testStatus;
}

void StatusEffects::ApplyStatusEffect(StatusFlags applyStatuses)
{
    statuses |= applyStatuses;
}

void StatusEffects::ClearStatusEffect(StatusFlags clearStatuses)
{
    statuses &= ~clearStatuses;
}

void StatusEffects::Init()
{
    statuses = 0;
}

void StatusEffects::Save(ostream& ofs) const
{
    ofs << "statuses: " << (int)statuses << '\n';
}

void StatusEffects::Load(istream& ifs)
{
    int statusesInt;
    ifs.ignore(16, ':') >> statusesInt;
    statuses = (char)statusesInt;
}
