#pragma once
#include "Utility.h"

// It's safe to pass this around raw because it's just two integers and a pointer
struct Image
{
    void Alloc(uint32_t width, uint32_t height)
    {
        this->width = width;
        this->height = height;
        data = new Color[width * height];
    }

    void Free()
    {
        delete[] data;
    }

    uint32_t GetDataSize() const
    {
        return width * height;
    }

    uint32_t width = 0;
    uint32_t height = 0;
    Color* data = nullptr;
};

Image LoadImageFromBitmap(const char* filename);
void UnloadImage(Image image);

constexpr char AsciiGrayscale(float value);

void DrawColoredText(const char* text, Color color);
void DrawBlock(Color color);
void DrawBitmap(const char* filename);
