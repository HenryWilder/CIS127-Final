#pragma once
#include "Utility.h"

// It's safe to pass this around raw because it's just two integers and a pointer
struct Image
{
    unsigned width;
    unsigned height;
    Color* data = nullptr;
};

Image LoadImageFromBitmap(const char* filename);
void UnloadImage(Image image);

constexpr char AsciiGrayscale(float value);

void DrawColoredText(const char* text, Color color);
void DrawBlock(Color color);
void DrawBitmap(const char* filename);
