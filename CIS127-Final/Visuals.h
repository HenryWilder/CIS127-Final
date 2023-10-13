#pragma once
#include <sal.h>
#include <functional>

constexpr char AsciiGrayscale(float value);

struct Vector2
{
    float x, y;
};

class TextureGrayscale;
using FragShader = std::function<float(const TextureGrayscale& texture0, Vector2 fragTexCoord)>;

class TextureGrayscale
{
public:
    TextureGrayscale(size_t width, size_t height);
    ~TextureGrayscale();

    void Print() const;

    // fragTexCoord is [{0,0}..{1,1}]
    void ApplyFragmentShader(FragShader fragShader);

    size_t GetWidth() const;
    size_t GetHeight() const;

    float at(_In_range_(0, width - 1) size_t x, _In_range_(0, width - 1) size_t y) const;
    float at(Vector2 coord) const;

    void UseWrappedUVs();
    void UseClampedUVs();

private:
    // [{0,0}..{width,height})
    float& at(_In_range_(0, width - 1) size_t x, _In_range_(0, height - 1) size_t y);

    // [{0,0}..{1,1}]
    float& at(float x, float y);

    // If false, clamps coords. If true, wraps coords.
    bool isWrapped = false;
    const size_t width;
    const size_t height;
    _Field_size_full_(width * height) float* const data;
};
