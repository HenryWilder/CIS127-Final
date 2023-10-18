#include "Visuals.h"
#include <iostream>
#include <fstream>
#include <ConsoleGraphics.h>

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

        data = std::shared_ptr<Color>(new Color[(size_t)width * (size_t)height]);
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
#if 0 // Debug color visuals
        Print();
#endif
    }
}

#undef DEBUG_PRINT

void DrawColoredText(const char* text, Color color)
{
    printf("\x1B[38;2;%d;%d;%dm%s\x1B[0m",
        (int)color.r, (int)color.g, (int)color.b,
        text);
}

Image::Image() :
    width(0), height(0), data(nullptr) {}

void Image::Unload()
{
    data.reset();
}

void Image::Print() const
{
    cg::Init(width, height);
    cg::BeginDrawing();
    cg::ClearBackground(BLACK);

    Color* dataPtr = data.get();
    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            Color color = dataPtr[y * width + x];
            cg::DrawPixel(vec2((float)x, (float)y), color);
        }
    }

    cg::EndDrawing();
    cg::Finished();
}

constexpr size_t NUM_SUPERSAMPLE_POINTS = 6;
void GetPlantersPoints(_Outref_ vec2 (&points)[NUM_SUPERSAMPLE_POINTS], vec2 max)
{
    constexpr float PLANTER_POINT_RADIUS = 0.0625; // When 0-1
    constexpr float PLANTER_POINT_RADIUS_SQR = PLANTER_POINT_RADIUS * PLANTER_POINT_RADIUS; // a^2 * b^2 = c^2, and square is cheaper than root

    // The Above assumption is proven by these assertions
    static_assert(NearlyEqual(cx::sqrt(sqr(3.0f) + sqr(4.0f)),     5.0f ));
    static_assert(NearlyEqual(        (sqr(3.0f) + sqr(4.0f)), sqr(5.0f)));

    for (size_t i = 0; i < NUM_SUPERSAMPLE_POINTS; ++i)
    {
        bool isPointTooClose = true;
        vec2 point;
        while (isPointTooClose)
        {
            point = vec2::rand01();

            // Search existing points for collisions
            isPointTooClose = false;
            for (size_t j = 0; j < i; ++j)
            {
                if (distanceSqr(point, points[j]) < PLANTER_POINT_RADIUS_SQR)
                {
                    isPointTooClose = true;
                    break;
                }
            }
        }
        points[i] = point;
    }

    // Scale sample points
    for (vec2& point : points)
    {
        point *= max;
    }
}

void Image::Print(float scale, SamplerParams params) const
{
    bool isSuperSampled = params.filtering == FilterMethod::PLANTERS_AVERAGE;

    vec2 size = Size<vec2>();
    vec2 outSize = size * scale;
    vec2 incr = vec2(1) / outSize;

    vec2 superSamplePoints[NUM_SUPERSAMPLE_POINTS] = {};
    if (isSuperSampled)
    {
        params.filtering = FilterMethod::BILINEAR;
        GetPlantersPoints(superSamplePoints, incr);
    }

    cg::Init((size_t)outSize.x, (size_t)outSize.y);
    cg::BeginDrawing();
    cg::ClearBackground(BLACK);

    vec2 uv(0);
    for (uv.y = 0.0f; uv.y <= 1.0f; uv.y += incr.y)
    {
        for (uv.x = 0.0f; uv.x <= 1.0f; uv.x += incr.x)
        {
            Color color;
            if (!isSuperSampled)
            {
                color = Sample(uv, params);
            }
            else
            {
                vec3 samples = vec3(0);
                for (size_t i = 0; i < NUM_SUPERSAMPLE_POINTS; ++i)
                {
                    samples += (vec3)Sample(uv + superSamplePoints[i], params);
                }
                samples /= NUM_SUPERSAMPLE_POINTS;
                color = (Color)samples;
            }
            cg::DrawPixel(vec2(uv.x, uv.y) * outSize, color);
        }
    }

    cg::EndDrawing();
    cg::Finished();
}

void Image::PrintEx(rect src, irect dest, vec2 scale, SamplerParams params) const
{
    bool isSuperSampled = params.filtering == FilterMethod::PLANTERS_AVERAGE;

    ivec2 size = dest.size();
    vec2 destSizeInv = 1.0f / (vec2)size;
    vec2 srcSizeInv = 1.0f / src.size();
    vec2 scaleInv = 1.0f / scale;

    vec2 superSamplePoints[NUM_SUPERSAMPLE_POINTS] = {};
    if (isSuperSampled)
    {
        params.filtering = FilterMethod::BILINEAR;
        GetPlantersPoints(superSamplePoints, scaleInv * srcSizeInv);
    }

    vec2 finalSize = (vec2)size * scaleInv * destSizeInv;
    cg::Init((size_t)finalSize.x, (size_t)finalSize.y);
    cg::BeginDrawing();
    cg::ClearBackground(BLACK);

    ivec2 px(0);
    for (px.y = 0; px.y < size.y; ++px.y)
    {
        for (px.x = 0; px.x < size.x; ++px.x)
        {
            vec2 uv = lerp(src.TopLeft(), src.BottomRight(), scaleInv * (vec2)px * destSizeInv) * srcSizeInv;
            Color color;
            if (!isSuperSampled)
            {
                color = Sample(uv, params);
            }
            else
            {
                vec3 samples = vec3(0);
                for (size_t i = 0; i < NUM_SUPERSAMPLE_POINTS; ++i)
                {
                    samples += (vec3)Sample(uv + superSamplePoints[i], params);
                }
                samples /= NUM_SUPERSAMPLE_POINTS;
                color = (Color)samples;
            }
            cg::DrawPixel(px, color);
        }
    }

    cg::EndDrawing();
    cg::Finished();
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

Color Image::Sample(vec2 uv, SamplerParams params) const noexcept
{
    uv.x = (params.xWrap) ? fmodf(uv.x, 1.0f) : saturate(uv.x);
    uv.y = (params.yWrap) ? fmodf(uv.y, 1.0f) : saturate(uv.y);
    vec2 fullCoord = uv * (vec2((float)width, (float)height) - 1);

    switch (params.filtering)
    {
    case FilterMethod::NEAREST_NEIGHBOR:
    {
        uint32_t x = (uint32_t)fullCoord.x;
        uint32_t y = (uint32_t)fullCoord.y;
        return _Sample(x, y);
    }
    default:
    case FilterMethod::BILINEAR:
    {
        vec2 t = fmod(fullCoord, 1.0f);
        irect pixel((int)floorf(fullCoord.x), (int)floorf(fullCoord.y), (int)ceilf(fullCoord.x), (int)ceilf(fullCoord.y));
        Color colorTop    = mix(_Sample(pixel.TopLeft   ()), _Sample(pixel.TopRight   ()), t.x);
        Color colorBottom = mix(_Sample(pixel.BottomLeft()), _Sample(pixel.BottomRight()), t.x);
        Color color = mix(colorTop, colorBottom, t.y);
        return color;
    }
    }
}

Color Image::_Sample(uint32_t x, uint32_t y) const
{
    return data.get()[(size_t)y * (size_t)width + (size_t)x];
}

Color Image::_Sample(ivec2 pt) const
{
    return _Sample(pt.x, pt.y);
}
