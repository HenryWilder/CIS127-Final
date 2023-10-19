#include "ConsoleGraphics.h"
#include <vector>
#include <stack>
#include <thread>
using std::vector;
using std::stack;

void Sleep(size_t ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

#include "config.h"

#define IS_USING_GRAYSCALE_ASCII (CHOSEN_COLOR_METHOD == COLOR_USE_GRAYSCALE_ASCII) // Grayscale ASCII art
#define IS_USING_ESCAPE_CODES    (CHOSEN_COLOR_METHOD == COLOR_USE_ESCAPE_CODES   ) // Color ASCII art
#define IS_USING_WINDOWS_CONTEXT (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT) // Windows pixel functions

#define IS_USING_ASCII_RAMP_STANDARD (ASCII_RAMP_STYLE == ASCII_RAMP_STANDARD) // ASCII ramp with wider range
#define IS_USING_ASCII_RAMP_SHORT    (ASCII_RAMP_STYLE == ASCII_RAMP_SHORT   ) // ASCII ramp with shorter, more convincing range
#define IS_USING_ASCII_RAMP_BLOCKS   (ASCII_RAMP_STYLE == ASCII_RAMP_BLOCKS  ) // ASCII ramp with solid blocks

#if IS_USING_WINDOWS_CONTEXT
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
HWND console;
HDC dc;
#endif

#if IS_USING_GRAYSCALE_ASCII
using Pixel_t = float;
constexpr Pixel_t ColorToPixel(Color color)
{
    return color.ToGray();
}
#elif IS_USING_ESCAPE_CODES
using Pixel_t = Color;
#define ColorToPixel(color) (color)
#elif IS_USING_WINDOWS_CONTEXT
using Pixel_t = COLORREF;
constexpr Pixel_t ColorToPixel(Color color)
{
    return RGB(color.r, color.g, color.b);
}
#else
#error selected color method is invalid
#endif // IS_USING_GRAYSCALE_ASCII / IS_USING_ESCAPE_CODES / IS_USING_WINDOWS_CONTEXT

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

    ivec2 Size() const noexcept
    {
        return ivec2((int)_width, (int)_height);
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

    size_t Index(ivec2 v) const { return Index((size_t)v.x, (size_t)v.y); }

    bool IsInRange(size_t x, size_t y) const noexcept
    {
        return
            (0ull <= x && x < _width) &&
            (0ull <= y && y < _height);
    }

    bool IsInRange(ivec2 v) const noexcept { return IsInRange((size_t)v.x, (size_t)v.y); }
    bool IsInRange( vec2 v) const noexcept { return IsInRange((size_t)roundf(v.x), (size_t)roundf(v.y)); }

private:
    void DrawPixel(size_t index, Pixel_t value, float depth)
    {
        if (_depth.at(index) < depth)
        {
            _color.at(index) = value;
        }
    }

    void DrawPixel(size_t index, Pixel_t value)
    {
        _color.at(index) = value;
    }

public:
    void DrawPixel(vec2 v, Pixel_t value, float depth)
    {
        if (IsInRange(v))
        {
            DrawPixel(Index(v), value, depth);
        }
    }

    void DrawPixel(vec2 v, Pixel_t value)
    {
        if (IsInRange(v))
        {
            DrawPixel(Index(v), value);
        }
    }

    void DrawPixel(ivec2 v, Pixel_t value, float depth)
    {
        if (IsInRange(v))
        {
            DrawPixel(Index(v), value, depth);
        }
    }

    void DrawPixel(ivec2 v, Pixel_t value)
    {
        if (IsInRange(v))
        {
            DrawPixel(Index(v), value);
        }
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
#if IS_USING_WINDOWS_CONTEXT && !USE_SLOW_FILL
        RECT rec = {};
        LONG y, x;

        system("cls");

        // Create space for the graphic
        for (y = 0; y < _height; ++y)
        {
            putchar('\n');
        }

        Sleep(30ull);
        
#else
        size_t y, x;
#endif // IS_USING_WINDOWS_CONTEXT && !USE_SLOW_FILL

        for (y = 0; y < _height; ++y)
        {
            for (x = 0; x < _width; ++x)
            {
                Pixel_t value = _color[Index(x, y)];
#if IS_USING_GRAYSCALE_ASCII

                constexpr char ASCII_RAMP[] =
#if   ASCII_RAMP_STYLE == ASCII_RAMP_STANDARD
                    R"() .'`^",:;Il!i><~+_-?][}{1)(|\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$)";
#elif ASCII_RAMP_STYLE == ASCII_RAMP_SHORT
                    " .:-=+*#%@";
#elif ASCII_RAMP_STYLE == ASCII_RAMP_BLOCKS
                   " \xB0\xB1\xB2\xDB";
#endif
                constexpr float ASCII_RAMP_MAX = sizeof(ASCII_RAMP) - 2;
                float valueIndex = roundf(clamp(value, 0.0f, 1.0f) * ASCII_RAMP_MAX);
                char valueChar = ASCII_RAMP[(size_t)valueIndex];
                for (int i = 0; i < 2; ++i)
                {
                    putchar(valueChar);
                }

#elif IS_USING_ESCAPE_CODES

                printf("\x1B[38;2;%d;%d;%dm\xDB\xDB\x1B[0m", (int)value.r, (int)value.g, (int)value.b);

#elif IS_USING_WINDOWS_CONTEXT

                constexpr int PIXELS_PER_BLOCK = 16;

#if USE_SLOW_FILL

                for (int dy = y * PIXELS_PER_BLOCK; dy < (y + 1) * PIXELS_PER_BLOCK; ++dy)
                {
                    for (int dx = x * PIXELS_PER_BLOCK; dx < (x + 1) * PIXELS_PER_BLOCK; ++dx)
                    {
                        SetPixelV(dc, dx, dy, value);
                    }
                }
#else

                HBRUSH brush = CreateSolidBrush(value);
                rec.right  = (rec.left = x * PIXELS_PER_BLOCK) + PIXELS_PER_BLOCK;
                rec.bottom = (rec.top  = y * PIXELS_PER_BLOCK) + PIXELS_PER_BLOCK;
                FillRect(dc, &rec, brush);
                DeleteObject(brush);

#endif // USE_SLOW_FILL

#endif // IS_USING_GRAYSCALE_ASCII / IS_USING_ESCAPE_CODES / IS_USING_WINDOWS_CONTEXT
            }
#if !IS_USING_WINDOWS_CONTEXT
            putchar('\n');
#endif // !IS_USING_WINDOWS_CONTEXT
        }
    }

private:
    size_t _width, _height;
    vector<Pixel_t> _color;
    vector<float> _depth;
};

PixelBuffer& CurrentBuffer()
{
    static PixelBuffer mainBuffer;
    return mainBuffer;
}

namespace cg
{
    void Init()
    {
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

    void BeginDrawing(size_t width, size_t height)
    {
        CurrentBuffer() = PixelBuffer(width, height);
    }

    void BeginDrawing(ivec2 size)
    {
        CurrentBuffer() = PixelBuffer(size.x, size.y);
    }

    void BeginDrawing(vec2 size)
    {
        CurrentBuffer() = PixelBuffer((size_t)ceilf(size.x), (size_t)ceilf(size.y));
    }

    void EndDrawing()
    {
        CurrentBuffer().Flush();
    }

    void ClearBackground(Color color)
    {
        CurrentBuffer().Clear(ColorToPixel(color));
    }

    void DrawPixel(ivec2 position, Color color)
    {
        CurrentBuffer().DrawPixel(position, ColorToPixel(color)); 
    }

    void DrawPixel(vec2 position, Color color)
    {
        CurrentBuffer().DrawPixel(position, ColorToPixel(color)); 
    }

    void DrawRectangle(irect rec, Color color)
    {
        CurrentBuffer().DrawRectangle(rec, ColorToPixel(color));
    }

    void DrawRectangle(rect rec, Color color)
    {
        irect irec = {
            (int)round(rec.xmin),
            (int)round(rec.ymin),
            (int)round(rec.xmax),
            (int)round(rec.ymax)
        };
        DrawRectangle(irec, color);
    }

    void DrawTriangle(triangle rec, Color color)
    {
        // todo
    }

    ivec2 GetRenderSize()
    {
        return CurrentBuffer().Size();
    }
}
