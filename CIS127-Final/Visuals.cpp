#include "Visuals.h"
#include <iostream>
#include <fstream>

#if 0
#define DEBUG_PRINT(x) std::cout << #x ": " << x << '\n'
#else
#define DEBUG_PRINT(x)
#endif

// See https://en.wikipedia.org/wiki/BMP_file_format
void Image::LoadFromBitmap(const char* filename)
{
    // Assertions are used to ensure that the bitmaps used are the ones made specifically for this project

    std::ifstream bitmap(filename, std::ifstream::in | std::ifstream::binary);
    if (bitmap)
    {
        constexpr size_t headerSize = 54;
        char header[headerSize];
        bitmap.read(header, headerSize);

#if 1 // Checks
        {
            _ASSERT(header[0] == 'B' && header[1] == 'M');
            uint32_t sizeOfHeader    = *reinterpret_cast<uint32_t*>(header + 14); DEBUG_PRINT(sizeOfHeader);
            uint32_t compression     = *reinterpret_cast<uint32_t*>(header + 30); DEBUG_PRINT(compression);     _ASSERT(compression  ==  0);
            uint32_t paletteSize     = *reinterpret_cast<uint32_t*>(header + 46); DEBUG_PRINT(paletteSize);     _ASSERT(paletteSize  ==  0);
            uint32_t importantColors = *reinterpret_cast<uint32_t*>(header + 50); DEBUG_PRINT(importantColors); _ASSERT(importantColors  ==  0);
        }
#endif
        uint32_t size = *reinterpret_cast<uint32_t*>(header + 2); DEBUG_PRINT(size);
        //uint32_t offset = *reinterpret_cast<uint32_t*>(header + 10); DEBUG_PRINT(offset);
        width  = (uint32_t)*reinterpret_cast< int32_t*>(header + 18); DEBUG_PRINT(width);
        height = (uint32_t)*reinterpret_cast< int32_t*>(header + 22); DEBUG_PRINT(height);
        uint16_t bitsPerPixel = *reinterpret_cast<uint16_t*>(header + 28); DEBUG_PRINT(bitsPerPixel); _ASSERT(bitsPerPixel == 24);
        uint32_t dataSize = *reinterpret_cast<uint32_t*>(header + 34); DEBUG_PRINT(dataSize);
        uint32_t rowSize = (uint32_t)ceil((bitsPerPixel * width) / 32.0f) * 4; DEBUG_PRINT(rowSize);

        _ASSERT(dataSize % rowSize == 0);
        _ASSERT(dataSize % 4 == 0);
        _ASSERT(dataSize >= 3);

        char* fileData = new char[dataSize];
        bitmap.read(fileData, dataSize);

#if 0 // Debug file contents
        {
            size_t x = 0;
            for (uint32_t i = 0; i < headerSize + dataSize; ++i)
            {
                int value = i < headerSize
                    ? (unsigned char)header[i]
                    : (unsigned char)fileData[i - headerSize];

                printf("%02X ", value);
                ++x;
                if (x == 8)
                {
                    std::cout << ' ';
                }
                if (x == 16)
                {
                    std::cout << '\n';
                    x = 0;
                }
            }
            std::cout << '\n' << std::endl;
        }
#endif

#if 0 // Debug pixel array
        {
            size_t x = 0;
            for (uint32_t i = 0; i < dataSize; ++i)
            {
                int value = (unsigned char)fileData[i];
                printf("%02X ", value);
                ++x;
                if (x == 8)
                {
                    std::cout << ' ';
                }
                if (x == 16)
                {
                    std::cout << '\n';
                    x = 0;
                }
            }
            std::cout << '\n';
        }
#endif

        data = std::shared_ptr<Color>(new Color[width * height]);
        Color* dataPtr = this->data.get();

        // Load data into memory
        {
            size_t i = 0;
            for (int y = height - 1; y >= 0; --y)
            {
                uint32_t rowStart = (uint32_t)y * rowSize;
                for (uint32_t x = 0; x < width; ++x)
                {
                    uint32_t pixelStart = rowStart + x * 3 + 2;
                    byte r = fileData[pixelStart - 0];
                    byte g = fileData[pixelStart - 1];
                    byte b = fileData[pixelStart - 2];
                    dataPtr[i] = Color(r, g, b);
                    ++i;
                }
            }
        }
        delete[] fileData; // Frees file content without freeing the image we have stored
        bitmap.close();

#if 0 // Debug color values
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Color color = dataPtr[y * width + x];
                printf("[%02X %02X %02X]", (int)color.r, (int)color.g, (int)color.b);
            }
            std::cout << '\n';
        }
#endif
#if 1 // Debug color visuals
        Print();
#endif
    }
}

#undef DEBUG_PRINT

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

Image::Image() :
    width(0), height(0), data(nullptr) {}

void Image::Unload()
{
    data.reset();
}

uint32_t Image::Size() const
{
    return width * height;
}

void Image::Print() const
{
    Color* dataPtr = data.get();
    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            Color color = dataPtr[y * width + x];
            DrawBlock(color);
        }
        std::cout << '\n';
    }
}

Image::operator bool() const
{
    return (bool)data;
}

bool Image::operator==(const Image& other) const
{
    return data == other.data;
}

Image& Image::operator=(const Image& other)
{
    width  = other.width;
    height = other.height;
    data   = other.data;
    return *this;
}
