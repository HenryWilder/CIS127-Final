#include "Health.hpp"

int Health::Get() const
{
    return health;
}

int Health::GetMax() const
{
    return maxHealth;
}

bool Health::IsDead() const
{
    return health <= 0;
}

void Health::Set(int health, int maxHealth)
{
    this->health    = health;
    this->maxHealth = maxHealth;
}

int Health::Heal(int points)
{
    assert(points > 0);
    assert(health <= maxHealth);
    int possibleToHeal = maxHealth - health;
    if (possibleToHeal <= points)
    {
        health += points;
        return 0;
    }
    else
    {
        health = maxHealth;
        return points - possibleToHeal;
    }
}

bool Health::Damage(int points)
{
    assert(points > 0);
    if (health >= points)
    {
        health -= points;
    }
    else
    {
        health = 0;
    }
    return health != 0;
}
