#pragma once
#include "Utility.h"
#include <memory>
#include "vec2.h"

enum class FilterMethod
{
    NEAREST_NEIGHBOR = 0,
    BILINEAR,
};

struct SamplerParams
{
    bool xWrap = false; // X is clamped if false
    bool yWrap = false; // Y is clamped if false
    FilterMethod filtering = FilterMethod::NEAREST_NEIGHBOR;
};

// It's safe to pass this around raw because it's just two integers and a shared pointer.
class Image
{
public:
    Image();

    void LoadFromBitmap(const char* filename);
    void Unload();
    uint32_t Size() const;
    void Print() const;
    void Print(float scale) const;
    operator bool() const;
    bool operator==(const Image& other) const;
    Image& operator=(const Image& other);

    Color Sample(vec2 uv, SamplerParams params = {}) const noexcept;

private:
    Color _Sample(uint32_t x, uint32_t y) const;
    Color _Sample(ivec2 pt) const;

    uint32_t width;
    uint32_t height;
    std::shared_ptr<Color> data;
};

constexpr char AsciiGrayscale(float value);

void DrawColoredText(const char* text, Color color);
void DrawBlock(Color color);
