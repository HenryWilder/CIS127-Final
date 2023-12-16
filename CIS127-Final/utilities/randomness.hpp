#pragma once
#include "utilities/pch/utilities.hpp"

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

/**
 * Generates a random number between the specified minimum and maximum values.
 *
 * @param min The minimum value of the range (inclusive).
 * @param max The maximum value of the range (inclusive).
 * @return A random number between min and max.
 */
int RandomNumber(int min, int max);

/**
 * @brief Roll a dice. The roll will succeed on average [chance] times in every [outOf] rolls.
 *
 * @param chance The highest number that will result in a success.
 *               (`chance == 0` will always fail; `chance >= outOf` will always succeed).
 * @param outOf The total number of sides on the dice.
 * @return True if the random event occurs, false otherwise.
 */
bool DiceCheck(int chance, int outOf);

/**
 * @brief Determines the success of an event based on a given chance.
 *
 * @param chance01 The probability of the event occurring, ranging from 0 to 1.
 * @return True if the event succeeds, false otherwise.
 */
bool Chance(float chance01);

/**
 * @brief Flips a coin and determines the success based on heads or tails.
 *
 * The event succeeds 50% of the time.
 *
 * @return True if heads, false if tails.
 */
bool CoinFlip();

/**
 * @brief Flips two coins and determines the success based on at least one heads.
 *
 * The event succeeds 75% of the time.
 *
 * @return True if at least one heads, false if both are tails.
 */
bool AdvantagedCoinFlip();

/**
 * @brief Flips two coins and determines the success based on both being heads.
 *
 * The event succeeds 25% of the time.
 *
 * @return True if both are heads, false if either is tails.
 */
bool DisadvantagedCoinFlip();
