#pragma once

enum class RollWith
{
    DISADVANTAGE = -1,
    NORMAL       =  0,
    ADVANTAGE    = +1,
};

enum Die : unsigned
{
    D2 = 2,
    D4 = 4,
    D6 = 6,
    D8 = 8,
    D10 = 10,
    D20 = 20,
    D100 = 100,
};

int Roll(unsigned n, Die d, int modifier = 0, RollWith rollWith = RollWith::NORMAL);
