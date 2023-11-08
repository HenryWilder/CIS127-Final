#pragma once
#include "utilities.hpp"

template<input_iterator _It>
const auto& ChooseRandom(_It optionsBegin, _It optionsEnd)
{
    assert(optionsBegin != optionsEnd);
    _It optionsRand = optionsBegin;
    advance(optionsRand, rand() % distance(optionsBegin, optionsEnd));
    return *optionsRand;
}

template<class _Container>
const auto& ChooseRandom(const _Container& options)
{
    return ChooseRandom(begin(options), end(options));
}

template<class _Ty>
const _Ty& ChooseRandom(const initializer_list<_Ty>&& options)
{
    return ChooseRandom(options);
}

template<class _Container>
const auto& ChooseRandomKey(const _Container& associativeOptions)
{
    return ChooseRandom(associativeOptions).first;
}

template<class _Container>
const auto& ChooseRandomValue(const _Container& associativeOptions)
{
    return ChooseRandom(associativeOptions).second;
}

#if 0 // Not sure why this isn't working...
template<class... _Args>
auto ChooseRandom(_Args&&... options)
    requires(sizeof...(_Args) > 1 && all_same<_Args...>)
{
    using _Ty = common_type_t<_Args...>;
    constexpr size_t numArgs = sizeof...(_Args);
    return ChooseRandom(array<_Ty, numArgs>{ options... });
}
#endif

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
