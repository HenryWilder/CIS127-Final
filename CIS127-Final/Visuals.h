#pragma once
#include "Utility.h"
#include <memory>

// It's safe to pass this around raw because it's just two integers and a pointer.
// Once one instance of a particular image is freed, all instances of that image are freed.
// It is important that this is tracked so that deallocated memory isn't accidentally accessed.
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
