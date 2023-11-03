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
