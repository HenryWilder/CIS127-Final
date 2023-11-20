#include "utilities.hpp"
#include "Health.hpp"
#include "TurnEchoStream.hpp"

int Health::Heal(int points)
{
    assert(points > 0);
    assert(health <= maxHealth);
    int possibleToHeal = maxHealth - health;
    if (possibleToHeal <= points)
    {
        health += points;
    }
    else
    {
        health = maxHealth;
    }
    echo << "[You gained " << points << " health.]\n";
    return points - possibleToHeal;
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
    echo << "[You took " << points << " damage.]\n";
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
