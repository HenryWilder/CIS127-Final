#include "RNG.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using std::vector;
using std::cout;
using std::endl;

#define DEBUG_ROLL 1

int Roll(unsigned n, Die d, int modifier, RollWith rollWith)
{
#if DEBUG_ROLL
    cout << n << "d" << d << (modifier >= 0 ? "+" : "") << modifier;
    switch (rollWith)
    {
    case RollWith::DISADVANTAGE: cout << " DIS"; break;
    case RollWith::ADVANTAGE:    cout << " ADV"; break;
    }
    cout << ": ";
#endif

    bool isNormalRoll = rollWith == RollWith::NORMAL;
    size_t totalRolls = isNormalRoll ? n : n * 2ull;

    vector<int> rolls;
    rolls.reserve(totalRolls);
    for (size_t i = 0; i < totalRolls; ++i)
    {
        rolls.push_back(rand() % d + 1);
    }

#if DEBUG_ROLL
    cout << "Rolling... ";
    for (int roll : rolls)
    {
        cout << roll << ' ';
    }
#endif

    if (!isNormalRoll)
    {
        std::sort(rolls.begin(), rolls.end());

        switch (rollWith)
        {
        case RollWith::ADVANTAGE:
            rolls.erase(rolls.begin(), rolls.begin() + n);
            break;

        case RollWith::DISADVANTAGE:
            rolls.erase(rolls.begin() + n, rolls.end());
            break;
        }

#if DEBUG_ROLL
        cout << " Remaining half: ";
        for (int roll : rolls)
        {
            cout << roll << ' ';
        }
#endif
    }
    cout << endl;
        
    return std::accumulate(rolls.begin(), rolls.end(), modifier);
}
