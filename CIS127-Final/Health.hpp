#pragma once
#include "IPlayerComponent.hpp"

class Health :
    IPlayerComponent
{
public:
    int GetHealth( ) const
    {
        return health;
    }

    int GetMaxHealth( ) const
    {
        return maxHealth;
    }

    bool IsDead( ) const
    {
        return health <= 0;
    }

    // Returns quantity of health points exceeding the max health (and not added).
    // If all points were added successfully, returns 0.
    int Heal(int points);

    // Returns true if survived, returns false if dead.
    bool Damage(int points);

    void Init( ) override;
    void Save(ostream &ofs) const override;
    void Load(istream &ifs) override;

private:
    int health = 0;
    int maxHealth = 0;
};
