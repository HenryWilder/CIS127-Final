#ifndef Influences_hpp
#define Influences_hpp

#include "utilities.hpp"
#include "Enums.hpp"

// Encompasses power, faith, and status; [-10..+10]
class Influences :
    private NotCopyable
{
public:
    // Returns the influence associated with the target collective.
    int Get(Collective targetCollective) const;
    
    // Returns the iterable collection of all influences.
    const map<Collective, int>& GetAll() const;
    
    // Increases/decreases social status among the targeted collective (shortname) by the provided amount.
    // Clamps influence to range [-10..+10]; -50%, 200%, etc. success would be undefined behavior.
    void Modify(Collective targetCollective, int change);
    
    // Erases all influence, effectively setting them to 0
    void ClearAll();
    
    // Roll a dice for a social check such that:
    // -10 influence gives a   0% chance of success
    //   0 influence gives a  50% chance of success
    // +10 influence gives a 100% chance of success
    bool Check(Collective targetCollective) const;
    
    void Init();
    void Save(ostream& ofs) const;
    void Load(istream& ifs);
    
private:
    map<Collective, int> influence;
};

#endif /* Influences_hpp */
