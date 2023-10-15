#pragma once
#include "Utility.h"
#include <memory>

// It's safe to pass this around raw because it's just two integers and a shared pointer.
class Image
{
public:
    Image();

    void LoadFromBitmap(const char* filename);
    void Unload();
    uint32_t Size() const;
    void Print() const;
    operator bool() const;
    bool operator==(const Image& other) const;
    Image& operator=(const Image& other);

private:
    uint32_t width;
    uint32_t height;
    std::shared_ptr<Color> data;
};

constexpr char AsciiGrayscale(float value);

void DrawColoredText(const char* text, Color color);
void DrawBlock(Color color);
