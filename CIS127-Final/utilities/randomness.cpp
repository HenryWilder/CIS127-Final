#include "utilities/pch/utilities.hpp"
#include "randomness.hpp"

random_device rd;
mt19937 gen(rd( ));

int RandomNumber(int min, int max)
{
    return uniform_int(min, max)(gen);
}

bool DiceCheck(int chance, int outOf)
{
    return RandomNumber(1, outOf) <= chance;
}

bool Chance(float chance01)
{
    return uniform_real<float>( )(gen) <= chance01;
}

bool CoinFlip( )
{
    return (bool)uniform_int(0, 1)(gen);
}

bool AdvantagedCoinFlip( )
{
    return CoinFlip( ) || CoinFlip( );
}

bool DisadvantagedCoinFlip( )
{
    return CoinFlip( ) && CoinFlip( );
}
