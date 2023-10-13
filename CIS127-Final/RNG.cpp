#include "RNG.h"
#include <iostream>
#include <vector>
#include <algorithm>
using std::vector;

int Roll(Die d, unsigned n, int modifier, RollMethod rollWith)
{
    bool isNormalRoll = rollWith == RollMethod::NRM;
    size_t totalRolls = isNormalRoll ? n : n * 2ull;

    vector<int> rolls;
    rolls.reserve(totalRolls);
    for (size_t i = 0; i < totalRolls; ++i)
    {
        rolls.push_back(rand() % d);
    }

    if (!isNormalRoll)
    {
        std::sort(rolls.begin(), rolls.end()); // Need to test if this sorts ascending

        switch (rollWith)
        {
        case RollMethod::ADV:
            rolls.erase(rolls.begin(), rolls.begin() + n);
            break;

        case RollMethod::DIS:
            rolls.erase(rolls.begin() + n, rolls.end());
            break;
        }
    }

    int total = 0;
    for (int roll : rolls)
    {
        total += roll;
    }

    return total + modifier;
}
