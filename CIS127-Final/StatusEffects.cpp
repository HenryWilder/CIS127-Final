#include "utilities.hpp"
#include "StatusEffects.hpp"
#include "TurnEchoStream.hpp"

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
    echo << "[You gained the " << to_string(applyStatuses) << " status effect(s).]\n";
}

void StatusEffects::ClearStatusEffect(StatusFlags clearStatuses)
{
    statuses &= ~clearStatuses;
    echo << "[You lost the " << to_string(clearStatuses) << " status effect(s).]\n";
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
