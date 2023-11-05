#ifndef Health_hpp
#define Health_hpp

#include "utilities.hpp"
#include "StatusEffects.hpp"

class Health :
    private NotCopyable
{
public:
    int Get() const;
    
    int GetMax() const;
    
    bool IsDead() const;
    
    // Returns quantity of health points exceeding the max health (and not added).
    // If all points were added successfully, returns 0.
    int Heal(int points);
    
    // Returns true if survived, returns false if dead.
    bool Damage(int points);

    void Init();
    void Save(ostream& ofs) const;
    void Load(istream& ifs);
    
public:
    StatusEffects statuses;
    
private:
    int health    = 0;
    int maxHealth = 0;
};

#endif /* Health_hpp */
