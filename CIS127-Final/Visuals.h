#pragma once
#include "Shapes.h"
#include <sal.h>
#include <functional>
#include <vector>
using std::vector;

constexpr char AsciiGrayscale(float value);

class TextureGrayscale;
using FragShader_Simple = std::function<float(vec2 fragTexCoord)>;
using FragShader = std::function<float(const TextureGrayscale& texture0, vec2 fragTexCoord)>;

class TextureGrayscale
{
public:
    TextureGrayscale(size_t width, size_t height);

    void Print(float scale = 1.0f) const;
    void PrintIso(float scale = 1.0f) const;
    void Draw(const TextureGrayscale& src, quad quad);

    // fragTexCoord is [{0,0}..{1,1}]
    void ApplyFragmentShader(FragShader_Simple fragShader);
    // fragTexCoord is [{0,0}..{1,1}]
    void ApplyFragmentShader(FragShader fragShader);

    size_t GetWidth() const;
    size_t GetHeight() const;

    float at(_In_range_(0, width - 1) size_t x, _In_range_(0, width - 1) size_t y) const;
    float at(vec2 coord) const;

    void UseWrappedUVs();
    void UseClampedUVs();

private:
    // [{0,0}..{width,height})
    float& at(_In_range_(0, width - 1) size_t x, _In_range_(0, height - 1) size_t y);

    // [{0,0}..{1,1}]
    float& at(vec2 coord);

    // If false, clamps coords. If true, wraps coords.
    bool isWrapped = false;
    const size_t width;
    const size_t height;
    vector<float> data;
};

constexpr float PI = 3.14159265359f;
constexpr float PI2 = PI * 2.0f;

namespace shader_presets
{
    template<float brightness>
    FragShader_Simple clearBackground = +[](vec2 fragTexCoord)
    {
        return brightness;
    };

    template<float _Directions, float _Quality, float _Radius>
    FragShader blur = +[](const TextureGrayscale& texture0, vec2 fragTexCoord)
    {
        constexpr float iIncrement = 1.0f / _Quality;
        constexpr float averager = _Quality * _Directions - 15.0f;
        float brightness = texture0.at(fragTexCoord);
        for (float d = 0.0f; d < PI2; d += PI2 / _Directions)
        {
            for (float i = iIncrement; i <= 1.0f; i += iIncrement)
            {
                vec2 offset = vec2(cosf(d), sinf(d)) * _Radius * i;
                brightness += texture0.at(fragTexCoord + offset);
            }
        }
        brightness /= averager;
        return brightness;
    };

    FragShader Circle(vec2 center, float radius, float gray);
    FragShader Rectangle(float x, float y, float width, float height, float gray);
}
