#pragma once
#include <HenryLib.h>
#include <memory>

enum class FilterMethod
{
    NEAREST_NEIGHBOR = 0,
    BILINEAR, // Useful for upscaling
    PLANTERS_AVERAGE, // Useful for downscaling
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

    constexpr uint32_t Area() const
    {
        return width * height;
    }

    template<series_of<2> _Vec>
    constexpr _Vec Size() const
    {
        using _Ty = typename _Vec::value_type;
        return _Vec((_Ty)width, (_Ty)height);
    }

    void Print() const;
    void Print(float scale, SamplerParams params = {}) const;
    // @param src - The region (in fractional pixels) of the image to sample
    // @param dest - The region of the console to output to (does not currently support position, only size)
    // @param scale - Extra scaling after resizing src to match dest
    void PrintEx(rect src, irect dest, vec2 scale, SamplerParams params = {}) const;
    operator bool() const;
    bool operator==(const Image& other) const;
    Image& operator=(const Image& other);

    Color Sample(vec2 uv, SamplerParams params) const noexcept;

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
