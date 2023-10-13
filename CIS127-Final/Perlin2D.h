#pragma once
#include <sal.h>
#include <cstdlib>
#include <iostream>
#include <vector>
using std::vector;
using std::cout;

constexpr bool IsPow2(size_t n)
{
    unsigned numBitsSet = 0;
    while (n)
    {
        numBitsSet += n & 1;
        if (numBitsSet > 1) return false;
        n >>= 1;
    }
    return numBitsSet == 1;
}
static_assert(IsPow2(1) == true);
static_assert(IsPow2(2) == true);
static_assert(IsPow2(3) == false);
static_assert(IsPow2(4) == true);
static_assert(IsPow2(6) == false);
static_assert(IsPow2(8) == true);

template<size_t _Size>
void GenPerlin2D(_Outref_ float(&data)[_Size * _Size])
{
    static_assert(_Size != 1, "Size cannot be 1");
    static_assert(IsPow2(_Size), "Size must be a power of 2");

    for (size_t density = 2; density <= _Size; density <<= 1)
    {
        size_t squareSize = _Size / density;

        // Apply offsets
        float range = 1.0f / density;
        for (size_t y = 0; y < density; ++y)
        {
            size_t yStart = y * squareSize;
            size_t yEnd = yStart + squareSize;

            for (size_t x = 0; x < density; ++x)
            {
                size_t xStart = x * squareSize;
                size_t xEnd = xStart + squareSize;

                float offset = (rand() / (float)RAND_MAX - 0.5f) * range;

                // Apply offsets
                for (size_t py = yStart; py < yEnd; ++py)
                {
                    for (size_t px = xStart; px < xEnd; ++px)
                    {
                        data[py * _Size + px] += offset;
                    }
                }
            }
        }

        // Apply blurring
        for (size_t py = 0; py < _Size; ++py)
        {
            size_t oyStart = py - squareSize;
            if (oyStart < 0) oyStart = 0;

            size_t oyEnd = py + squareSize;
            if (oyEnd > _Size) oyEnd = _Size;

            for (size_t px = 0; px < _Size; ++px)
            {
                size_t oxStart = px - squareSize;
                if (oxStart < 0) oxStart = 0;

                size_t oxEnd = px + squareSize;
                if (oxEnd > _Size) oxEnd = _Size;

                float sum = 0.0f;
                size_t areaSummed = 0;
                for (size_t oy = oyStart; oy < oyEnd; ++oy)
                {
                    for (size_t ox = oxStart; ox < oxEnd; ++ox)
                    {
                        sum += data[oy * _Size + ox];
                        ++areaSummed;
                    }
                }
                if (areaSummed == 0) continue;
                data[py * _Size + px] = sum / areaSummed;
            }
        }

        // Debug
    }
}
