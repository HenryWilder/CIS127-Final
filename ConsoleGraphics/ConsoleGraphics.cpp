#include "ConsoleGraphics.h"
#include <vector>
#include <stack>
using std::vector;
using std::stack;

// The whole reason I made this is so you can recompile the dll with
// a different configuration depending on what works for your system.

#define COLOR_USE_GRAYSCALE_ASCII 0x0
#define COLOR_USE_ESCAPE_CODES    0x1
#define COLOR_USE_WINDOWS_CONTEXT 0x2

// How pixels are colored
#define CHOSEN_COLOR_METHOD COLOR_USE_WINDOWS_CONTEXT

#if (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT)
// Toggle by commenting/uncommenting this macro
//#define USE_SLOW_FILL
#endif


#if (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
HWND console;
HDC dc;
#endif

#if (CHOSEN_COLOR_METHOD == COLOR_USE_GRAYSCALE_ASCII)
using Pixel_t = float;
constexpr Pixel_t ColorToPixel(Color color)
{
    return color.ToGray();
}
#elif (CHOSEN_COLOR_METHOD == COLOR_USE_ESCAPE_CODES)
using Pixel_t = Color;
#define ColorToPixel(color) (color)
#elif (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT)
using Pixel_t = COLORREF;
constexpr Pixel_t ColorToPixel(Color color)
{
    return RGB(color.r, color.g, color.b);
}
#endif // CHOSEN_COLOR_METHOD

class PixelBuffer
{
public:
    PixelBuffer() : _height(0), _width(0) {}

    PixelBuffer(size_t width, size_t height) :
        _width(width), _height(height), _color(Area()), _depth(Area()) {}

    void Clear(Pixel_t value = Pixel_t(0))
    {
        for (Pixel_t& pixel : _color)
        {
            pixel = value;
        }

        for (float& pixel : _depth)
        {
            pixel = INFINITY;
        }
    }

    size_t Area() const noexcept
    {
        return _width * _height;
    }

    size_t Index(_In_range_(0, _width - 1) size_t x, _In_range_(0, _height - 1) size_t y) const noexcept
    {
        return y * _width + x;
    }

    size_t Index(vec2 v) const
    {
        return Index((size_t)round(v.x), (size_t)round(v.y));
    }

    void DrawPixel(size_t index, Pixel_t value, float depth)
    {
        if (_depth.at(index) >= depth)
        {
            _color.at(index) = value;
        }
    }

    void DrawPixel(size_t index, Pixel_t value)
    {
        _color.at(index) = value;
    }

    void DrawPixel(vec2 v, Pixel_t value, float depth)
    {
        DrawPixel(Index(v), value, depth);
    }

    void DrawPixel(vec2 v, Pixel_t value)
    {
        DrawPixel(Index(v), value);
    }

    void DrawPixel(ivec2 v, Pixel_t value, float depth)
    {
        DrawPixel(Index(v.x, v.y), value, depth);
    }

    void DrawPixel(ivec2 v, Pixel_t value)
    {
        DrawPixel(Index(v.x, v.y), value);
    }

    void DrawPixel(vec3 v, Pixel_t value)
    {
        DrawPixel(Index(vec2(v.x, v.y)), value, v.z);
    }

    void DrawRectangle(irect rec, Pixel_t value, float depth)
    {
        rec = clamp(rec, 0, 0, (int)(_width  - 1), (int)(_height - 1));
        for (ivec2 px : rec)
        {
            DrawPixel(px, value, depth);
        }
    }

    void DrawRectangle(irect rec, Pixel_t value)
    {
        rec = clamp(rec, 0, 0, (int)(_width  - 1), (int)(_height - 1));
        for (ivec2 px : rec)
        {
            DrawPixel(px, value);
        }
    }

    void DrawTriangle(size_t index, Pixel_t value, float depth)
    {
        // todo
    }

    void DrawTriangle(size_t index, Pixel_t value)
    {
        // todo
    }

    void Flush()
    {
#if (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT) && !defined(USE_SLOW_FILL)
        RECT rec = {};
        LONG y, x;
#else
        size_t y, x;
#endif
        for (y = 0; y < _height; ++y)
        {
            for (x = 0; x < _width; ++x)
            {
                Pixel_t value = _color[Index(x, y)];
#if (CHOSEN_COLOR_METHOD == COLOR_USE_GRAYSCALE_ASCII)
                constexpr char ASCII_RAMP[] = { ' ', '\xB0', '\xB1', '\xB2', '\xDB' };
                constexpr float ASCII_RAMP_MAX = sizeof(ASCII_RAMP) - 1;
                float valueIndex = roundf(clamp(value, 0.0f, 1.0f) * ASCII_RAMP_MAX);
                char valueChar = ASCII_RAMP[(size_t)valueIndex];
                for (int i = 0; i < 2; ++i) putchar(valueChar); // Do twice to make a square
#elif (CHOSEN_COLOR_METHOD == COLOR_USE_ESCAPE_CODES)
                printf("\x1B[38;2;%d;%d;%dm\xDB\xDB\x1B[0m", (int)value.r, (int)value.g, (int)value.b);
#elif (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT)
#ifdef USE_SLOW_FILL
                for (int dy = y * 8; dy < (y + 1) * 8; ++dy)
                {
                    for (int dx = x * 8; dx < (x + 1) * 8; ++dx)
                    {
                        SetPixelV(dc, dx, dy, value);
                    }
                }
#else
                HBRUSH brush = CreateSolidBrush(value);
                rec.right  = (rec.left = x * 8) + 8;
                rec.bottom = (rec.top  = y * 8) + 8;
                FillRect(dc, &rec, brush);
                DeleteObject(brush);
#endif
#endif // (CHOSEN_COLOR_METHOD == COLOR_USE_GRAYSCALE_ASCII)
            }
            putchar('\n');
        }
    }

private:
    size_t _width, _height;
    vector<Pixel_t> _color;
    vector<float> _depth;
};

PixelBuffer mainBuffer;
stack<PixelBuffer*> bufferStack;

PixelBuffer& CurrentBuffer()
{
    return *bufferStack.top();
}

namespace cg
{
    void Init(size_t width, size_t height)
    {
        mainBuffer = PixelBuffer(width, height);
#if (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT)
        console = GetConsoleWindow();
        dc = GetDC(console);
#endif
    }

    void Finished()
    {
#if (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT)
        ReleaseDC(console, dc);
#endif
    }

    void BeginDrawing()
    {
        bufferStack.push(&mainBuffer);
    }

    void EndDrawing()
    {
        bufferStack.top()->Flush();
        bufferStack.pop();
    }

    void ClearBackground(Color color)
    {
        CurrentBuffer().Clear(ColorToPixel(color));
    }

    void DrawPixel(vec2 position, Color color)
    {
        CurrentBuffer().DrawPixel(position, ColorToPixel(color)); 
    }

    void DrawRectangle(rect rec, Color color)
    {
        irect irec = {
            (int)round(rec.xmin),
            (int)round(rec.ymin),
            (int)round(rec.xmax),
            (int)round(rec.ymax)
        };
        CurrentBuffer().DrawRectangle(irec, ColorToPixel(color));
    }

    void DrawTriangle(triangle rec, Color color)
    {
        // todo
    }
}
