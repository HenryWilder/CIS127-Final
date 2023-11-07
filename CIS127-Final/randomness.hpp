#pragma once
#include "utilities.hpp"
using namespace std;

template<random_access_iterator _It>
auto ChooseRandom(_It optionsBegin, _It optionsEnd)
{
    return *(optionsBegin + (rand() % (optionsEnd - optionsBegin)));
}

template<integral_indexable_container _Container>
const typename _Container::value_type& ChooseRandom(const _Container& options)
{
    return options[rand() % options.size()];
}

template<class _Ty>
_Ty ChooseRandom(const initializer_list<_Ty>&& options)
{
    return *(options.begin() + (rand() % options.size()));
}

// Roll a dice. The roll will succeed on average [chance] times in every [outOf] rolls.
bool DiceCheck(int chance, int outOf);

// Epsilon is 0.001.
// Chance less than epsilon will always fail.
// Chance greater than [1 minus epsilon] will always succeed.
bool Chance(float chance01);

// Flip a coin. Succeed if heads; fail if tails.
// Succeeds 50% of the time.
bool CoinFlip();

// Flip two coins. Succeed if either is heads; fail if both are tails.
// Succeeds 75% of the time.
bool AdvantagedCoinFlip();

// Flip two coins. Succeed if both are heads; fail if either is tails.
// Succeeds 25% of the time.
bool DisadvantagedCoinFlip();
