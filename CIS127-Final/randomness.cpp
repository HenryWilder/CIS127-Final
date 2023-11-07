#include "randomness.hpp"

bool DiceCheck(int chance, int outOf)
{
    return (rand() % outOf) < chance;
}

bool Chance(float chance01)
{
    if      (chance01 >= 0.999f) return true;
    else if (chance01 <= 0.001f) return false;
    return rand() <= (int)roundf(chance01 * (float)RAND_MAX);
}

bool CoinFlip()
{
    return rand() & 1;
}

bool AdvantagedCoinFlip()
{
    return rand() & 3;
}

bool DisadvantagedCoinFlip()
{
    return (rand() & 3) == 3;
}
