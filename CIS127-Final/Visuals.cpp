#include "Visuals.h"
#include <iostream>

#define USE_STANDARD_ASCII_RAMP 1

constexpr char AsciiGrayscale(float value)
{
    constexpr char ASCII_RAMP[] =
#if USE_STANDARD_ASCII_RAMP
        R"TXT( .'`^",:;Il!i><~+_-?][}{1)(|\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$)TXT";
#else
        R"TXT( .:-=+*#%@)TXT";
#endif
    constexpr size_t ASCII_RAMP_SIZE = sizeof(ASCII_RAMP) / sizeof(char) - 2; // -1 because of null terminator, -2 because [0..size)

    // clamping
    if (value > 1) value = 1;
    if (value < 0) value = 0;

    size_t index = (size_t)(value * ASCII_RAMP_SIZE + 0.5f);
    return ASCII_RAMP[index];
}
static_assert(AsciiGrayscale(0.0f) == ' ');
#if USE_STANDARD_ASCII_RAMP
static_assert(AsciiGrayscale(0.5f) == 'n');
static_assert(AsciiGrayscale(1.0f) == '$');
#else
static_assert(AsciiGrayscale(0.5f) == '+');
static_assert(AsciiGrayscale(1.0f) == '@');
#endif

TextureGrayscale::TextureGrayscale(size_t width, size_t height) :
    width(width), height(height), data(new float[width * height]) {}

TextureGrayscale::~TextureGrayscale()
{
    delete[] data;
}

void TextureGrayscale::Print() const
{
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            char ch = AsciiGrayscale(data[y * width + x]);
            std::cout << ch << ch;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void TextureGrayscale::Draw(const TextureGrayscale& src, quad quad)
{
    vec2 coord;
    for (coord.y = 0.0f; coord.y <= 1.0f; coord.y += 1.0f / src.GetHeight())
    {
        for (coord.x = 0.0f; coord.x <= 1.0f; coord.x += 1.0f / src.GetWidth())
        {
            vec2 position = quad.CoordToPos(coord);
            at(position) = src.at(coord);
        }
    }
}

void TextureGrayscale::ApplyFragmentShader(FragShader_Simple fragShader)
{
    const float xIncrement = 1.0f / width;
    const float yIncrement = 1.0f / height;
    vec2 coord = {};
    for (float y = 0.0f; y <= 1.0f; y += yIncrement)
    {
        coord.y = y;
        for (float x = 0.0f; x <= 1.0f; x += xIncrement)
        {
            coord.x = x;
            at(vec2(x, y))  = fragShader(coord);
        }
    }
}

void TextureGrayscale::ApplyFragmentShader(FragShader fragShader)
{
    TextureGrayscale texture0 = TextureGrayscale(width, height);
    for (size_t i = 0; i < width * height; ++i)
    {
        texture0.data[i] = data[i];
    }

    const float xIncrement = 1.0f / width;
    const float yIncrement = 1.0f / height;
    vec2 coord = {};
    for (float y = 0.0f; y <= 1.0f; y += yIncrement)
    {
        coord.y = y;
        for (float x = 0.0f; x <= 1.0f; x += xIncrement)
        {
            coord.x = x;
            at(vec2(x, y))  = fragShader(texture0, coord);
        }
    }
}

size_t TextureGrayscale::GetWidth() const
{
    return width;
}

size_t TextureGrayscale::GetHeight() const
{
    return height;
}

void TextureGrayscale::UseWrappedUVs()
{
    isWrapped = true;
}

void TextureGrayscale::UseClampedUVs()
{
    isWrapped = false;
}

_Use_decl_annotations_ float TextureGrayscale::at(size_t x, size_t y) const
{
    if (isWrapped)
    {
        x %= width;
        y %= height;
    }
    else
    {
        x = std::min(std::max(x, 0ull), width);
        y = std::min(std::max(y, 0ull), height);
    }
    return data[y * width + x];
}

float TextureGrayscale::at(vec2 coord) const
{
    return at(
        (size_t)roundf(coord.x * (width  - 1)),
        (size_t)roundf(coord.y * (height - 1))
    );
}

_Use_decl_annotations_ float& TextureGrayscale::at(size_t x, size_t y) 
{
    if (isWrapped)
    {
        x %= width;
        y %= height;
    }
    else
    {
        x = std::min(std::max(x, 0ull), width);
        y = std::min(std::max(y, 0ull), height);
    }
    return data[y * width + x];
}

float& TextureGrayscale::at(vec2 coord)
{
    return at(
        (size_t)roundf(coord.x * (width  - 1)),
        (size_t)roundf(coord.y * (height - 1))
    );
}

namespace shader_presets
{
    FragShader Circle(vec2 center, float radius, float gray)
    {
        auto fs = [center, radius, gray](const TextureGrayscale& texture0, vec2 fragTexCoord)
            {
                vec2 pt = fragTexCoord - center;
                bool isInCircle = (pt.x * pt.x + pt.y * pt.y) <= radius * radius;
                float brightness = isInCircle ? gray : texture0.at(fragTexCoord);
                return brightness;
            };

        return fs;
    }

    FragShader Rectangle(float x, float y, float width, float height, float gray)
    {
        float xmin = x;
        float xmax = x + width;
        float ymin = y;
        float ymax = y + height;
        auto fs = [xmin, xmax, ymin, ymax, gray](const TextureGrayscale& texture0, vec2 fragTexCoord)
            {
                bool isInRectagnle =
                    xmin <= fragTexCoord.x && fragTexCoord.x <= xmax &&
                    ymin <= fragTexCoord.y && fragTexCoord.y <= ymax;
                float brightness = isInRectagnle ? gray : texture0.at(fragTexCoord);
                return brightness;
            };

        return fs;
    }
}
