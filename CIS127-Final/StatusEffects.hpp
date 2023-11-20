#pragma once
#include "IPlayerComponent.hpp"

class StatusEffects :
    public IPlayerComponent
{
public:
    // Bitflags
    enum StatusFlags : char
    {
        Fire   = 1, // Cleared by water
        Poison = 2, // Cleared by antidote
        Tree   = 4, // Cleared by [TODO]
    };
    
    // Returns the combined set of all active status flags.
    char GetStatusEffects( ) const
    {
        return statuses;
    }
    
    // Returns true of all of the provided statuses are active.
    // Multiple statuses are tested at once by combining them with `|`.
    bool HasEveryStatusEffect(StatusFlags testStatuses) const;
    
    // Returns true of one or more of the provided statuses are active.
    // Multiple statuses are tested at once by combining them with `|`.
    bool HasAnyStatusEffect(StatusFlags testStatuses) const;
    
    // Alias for "HasAnyStatus()" to add clarity when testing only one status.
    bool HasStatusEffect(StatusFlags testStatus) const;
    
    // Sets the provided status(es) to true.
    // Multiple statuses can be set at once by combining them with `|`.
    void ApplyStatusEffect(StatusFlags applyStatuses);
    
    // Sets the provided status(es) to false.
    // Multiple statuses can be cleared at once by combining them with `|`.
    // Default: clears all statuses.
    void ClearStatusEffect(StatusFlags clearStatuses = (StatusFlags)~0);

    void Init() override;
    void Save(ostream& ofs) const override;
    void Load(istream& ifs) override;
    
private:
    char statuses = 0;
};
