#pragma once
#include "utilities.hpp"

extern mt19937 gen;

template<input_iterator _It = vector<string>::const_iterator>
_It ChooseRandomIterator(_It optionsBegin, const _It optionsEnd)
{
    using _Diff = _Iter_diff_t<_It>;

    assert(optionsBegin != optionsEnd); // Options cannot be empty
    const _Diff size = distance<_It>(optionsBegin, optionsEnd);
    assert(size > 0);
    const _Diff lastIndex = size - 1;
    const _Diff randomIndex = uniform_int<_Diff>(0, lastIndex)(gen);
    const _Diff offset = max<_Diff>(0, randomIndex);
    advance<_It, _Diff>(optionsBegin, offset);
    return optionsBegin;
}

template<iterable _Container>
auto ChooseRandom(const _Container& options)
    -> const typename _Container::value_type&
{
    return *ChooseRandomIterator(options.begin(), options.end());
}

template<iterable _Container = initializer_list<const char*>>
auto ChooseRandom(const _Container&& options)
    -> _Container::value_type
{
    return *ChooseRandomIterator(options.begin(), options.end());
}

template<associative_iterable _Container>
auto ChooseRandomKey(const _Container& associativeOptions)
    -> const decltype(associativeOptions.begin()->first)&
{
    return ChooseRandomIterator(associativeOptions.begin(), associativeOptions.end())->first;
}

template<associative_iterable _Container>
auto ChooseRandomValue(const _Container& associativeOptions)
    -> const decltype(associativeOptions.begin()->second)&
{
    return ChooseRandomIterator(associativeOptions.begin(), associativeOptions.end())->second;
}

template<class... _Tys>
common_type_t<_Tys...> ChooseRandom(_Tys&&... options)
    requires(sizeof...(_Tys) > 1)
{
    using _Ty = common_type_t<_Tys...>;
    using array_t = array<_Ty, sizeof...(_Tys)>;
    array_t arr = { options... };
    return ChooseRandom<array_t>(arr);
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
