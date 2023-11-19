#include "utilities.hpp"
#include "randomness.hpp"

random_device rd;
mt19937 gen(rd());

bool DiceCheck(int chance, int outOf)
{
    return uniform_int(1, outOf)(gen) <= chance;
}

bool Chance(float chance01)
{
    return uniform_real<float>()(gen) <= chance01;
}

bool CoinFlip()
{
    return (bool)uniform_int(0, 1)(gen);
}

bool AdvantagedCoinFlip()
{
    return CoinFlip() || CoinFlip();
}

bool DisadvantagedCoinFlip()
{
    return CoinFlip() && CoinFlip();
}
