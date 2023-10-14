#include "Visuals.h"
#include <iostream>

constexpr char AsciiGrayscale(float value)
{
    constexpr char ASCII_RAMP[] = { ' ', '\xB0', '\xB1', '\xB2', '\xDB', };
    constexpr size_t ASCII_RAMP_RANGE = sizeof(ASCII_RAMP) / sizeof(char) - 1;

    // clamping
    if (value > 1) value = 1;
    if (value < 0) value = 0;

    size_t index = (size_t)(value * ASCII_RAMP_RANGE + 0.5f);
    return ASCII_RAMP[index];
}
static_assert(AsciiGrayscale( 0.0f)  == ' ',    "Grayscale value of 0 should be empty");
static_assert(AsciiGrayscale( 0.1f)  == ' ',    "Grayscale value of 0.1 should round to 0");
static_assert(AsciiGrayscale( 0.2f)  == '\xB0', "Grayscale value of 0.2 should be the second char, which is '\xB0'");
static_assert(AsciiGrayscale( 0.15f) == '\xB0', "Grayscale value of 0.1 should round up to '\xB0'");
static_assert(AsciiGrayscale( 1.0f)  == '\xDB', "Grayscale value of 1.0 (maximum) should be '\xDB'");
static_assert(AsciiGrayscale(-1.0f)  == ' ',    "Grayscale values smaller than 0 should be clamped to 0");
static_assert(AsciiGrayscale( 2.0f)  == '\xDB', "Grayscale values exceeding 1 should be clamped to 1");

void DrawColoredText(const char* text, Color color)
{
    printf("\x1B[38;2;%d;%d;%dm%s\x1B[0m",
        (int)color.r, (int)color.g, (int)color.b,
        text);
}

void DrawBlock(Color color)
{
    DrawColoredText("\xDB\xDB", color);
}
