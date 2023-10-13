#pragma once

enum class RollMethod
{
    DIS = -1,
    NRM = 0,
    ADV = 1,
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

int Roll(Die d, unsigned n = 1, int modifier = 0, RollMethod rollWith = RollMethod::NRM);
