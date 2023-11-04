#ifndef StatusEffects_hpp
#define StatusEffects_hpp

#include "utilities.hpp"

class StatusEffects :
    private NotCopyable
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
    char Get() const;
    
    // Returns true of all of the provided statuses are active.
    // Multiple statuses are tested at once by combining them with `|`.
    bool HasEvery(StatusFlags testStatuses) const;
    
    // Returns true of one or more of the provided statuses are active.
    // Multiple statuses are tested at once by combining them with `|`.
    bool HasAny(StatusFlags testStatuses) const;
    
    // Alias for "HasAnyStatus()" to add clarity when testing only one status.
    bool Has(StatusFlags testStatus) const;
    
    // Sets the provided status(es) to true.
    // Multiple statuses can be set at once by combining them with `|`.
    void Apply(StatusFlags applyStatuses);
    
    // Sets the provided status(es) to false.
    // Multiple statuses can be cleared at once by combining them with `|`.
    // Default: clears all statuses.
    void Clear(StatusFlags clearStatuses = (StatusFlags)~0);

    void Init();
    void Save(ostream& ofs) const;
    void Load(istream& ifs);
    
private:
    char statuses;
};

#endif /* StatusEffects_hpp */
