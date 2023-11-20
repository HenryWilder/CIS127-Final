#include "utilities.hpp"
#include "Health.hpp"

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

void Health::Init()
{
    health    = 5;
    maxHealth = 10;
}

void Health::Save(ostream& ofs) const
{
    ofs << "health: " << health << " / " << maxHealth << '\n';
}

void Health::Load(istream& ifs)
{
    (ifs.ignore(16, ':') >> health).ignore(3, '/') >> maxHealth;
}
