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
    
    // Only for use when loading from file.
    void Set(int health, int maxHealth);
    
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
    int health, maxHealth;
};

#endif /* Health_hpp */
