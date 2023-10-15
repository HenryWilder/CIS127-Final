#include "Visuals.h"
#include <iostream>
#include <fstream>

#if 1
#define DEBUG_PRINT(x) std::cout << #x ": " << x << '\n'
#else
#define DEBUG_PRINT(x)
#endif

// See https://en.wikipedia.org/wiki/BMP_file_format
Image LoadImageFromBitmap(const char* filename)
{
    // Assertions are used to ensure that the bitmaps used are the ones made specifically for this project

    std::ifstream bitmap(filename, std::ifstream::in | std::ifstream::binary);
    if (bitmap)
    {
        char header[54];
        bitmap.read(header, sizeof(header));

        // Checks
#if 1
        {
            _ASSERT(header[0] == 'B' && header[1] == 'M');
            uint32_t sizeOfHeader    = *reinterpret_cast<uint32_t*>(header + 14); DEBUG_PRINT(sizeOfHeader);
            uint32_t compression     = *reinterpret_cast<uint32_t*>(header + 30); DEBUG_PRINT(compression);     _ASSERT(compression  ==  0);
            uint32_t paletteSize     = *reinterpret_cast<uint32_t*>(header + 46); DEBUG_PRINT(paletteSize);     _ASSERT(paletteSize  ==  0);
            uint32_t importantColors = *reinterpret_cast<uint32_t*>(header + 50); DEBUG_PRINT(importantColors); _ASSERT(importantColors  ==  0);
        }
#endif
        uint32_t size     = *reinterpret_cast<uint32_t*>(header +  2); DEBUG_PRINT(size);
        uint32_t offset   = *reinterpret_cast<uint32_t*>(header + 10); DEBUG_PRINT(offset);
         int32_t width    = *reinterpret_cast< int32_t*>(header + 18); DEBUG_PRINT(width);
         int32_t height   = *reinterpret_cast< int32_t*>(header + 22); DEBUG_PRINT(height);
        uint16_t bitsPerPixel = *reinterpret_cast<uint16_t*>(header + 28); DEBUG_PRINT(bitsPerPixel); _ASSERT(bitsPerPixel == 24);
        uint32_t dataSize = *reinterpret_cast<uint32_t*>(header + 34); DEBUG_PRINT(dataSize);
        int rowSize = ceil((bitsPerPixel * width) / 32) * 4; DEBUG_PRINT(rowSize);
        int paddingSize = rowSize - width; DEBUG_PRINT(paddingSize);

        char* data = new char[dataSize];
        bitmap.read(data, sizeof(data));

        unsigned x = 0;
        for (uint32_t i = 0; (i + 2) < dataSize; i += 3)
        {
            byte b = data[i + 0];
            byte g = data[i + 1];
            byte r = data[i + 2];
            printf("[%3d %3d %3d]", (int)r, (int)g, (int)b);
            ++x;
            if (x == width)
            {
                //i += paddingSize;
                x = 0;
                std::cout << '\n';
            }
        }

        std::cout << '\n';

        x = 0;
        for (uint32_t i = 0; (i + 2) < dataSize; i += 3)
        {
            byte b = data[i + 0];
            byte g = data[i + 1];
            byte r = data[i + 2];
            Color color(r, g, b);
            DrawBlock(color);
            ++x;
            if (x == width)
            {
                //i += paddingSize;
                x = 0;
                std::cout << '\n';
            }
        }

        delete[] data;
    }
    bitmap.close();
    return Image();
}

#undef DEBUG_PRINT

void UnloadImage(Image image)
{
    _ASSERT_EXPR(image.data != nullptr, "Attempted to double-unload an image");
    delete[] image.data;
}

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
