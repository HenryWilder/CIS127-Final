#include "utilities.hpp"
#include "StatusEffects.hpp"

char StatusEffects::Get() const
{
    return statuses;
}

bool StatusEffects::HasEvery(StatusFlags testStatuses) const
{
    return (statuses & testStatuses) == testStatuses;
}

bool StatusEffects::HasAny(StatusFlags testStatuses) const
{
    return statuses & testStatuses;
}

bool StatusEffects::Has(StatusFlags testStatus) const
{
    assert(IsExactlyOneBitSet(testStatus));
    return statuses & testStatus;
}

void StatusEffects::Apply(StatusFlags applyStatuses)
{
    statuses |= applyStatuses;
}

void StatusEffects::Clear(StatusFlags clearStatuses)
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
