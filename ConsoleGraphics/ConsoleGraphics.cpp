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

// Selection for method to use when coloring pixels
// 
// ! IMPORTANT: This is what determines the driver configuration.
#define CHOSEN_COLOR_METHOD COLOR_USE_GRAYSCALE_ASCII

#if CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT
#define WIN32_LEAN_AND_MEAN
namespace win {
#include <Windows.h>
}
#endif

namespace cg
{
#if (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT)
    using Pixel_t = float;
    constexpr Pixel_t ColorToPixel(Color color) { return color.ToGray(); }
#else
    using Pixel_t = Color;
#define ColorToPixel(color) (color)
#endif // CHOSEN_COLOR_METHOD

    class PixelBuffer
    {
    public:
        PixelBuffer(size_t width, size_t height) :
            _width(width), _height(height) {}

        size_t Index(size_t x, size_t y) const
        {
            return y * _width + x;
        }

        size_t Index(vec2 v) const
        {
            return Index((size_t)round(v.x), (size_t)round(v.y));
        }

        void DrawPixel(size_t index, Pixel_t value)
        {

        }

        void Flush()
        {
#if   (CHOSEN_COLOR_METHOD == COLOR_USE_GRAYSCALE_ASCII)

#elif (CHOSEN_COLOR_METHOD == COLOR_USE_ESCAPE_CODES)

#elif (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT)

#endif // CHOSEN_COLOR_METHOD
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

    void Init(size_t width, size_t height)
    {
        mainBuffer = PixelBuffer(width, height);
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

    void DrawPixel(vec2 position, Color color)
    {
        PixelBuffer& buff = CurrentBuffer();
        buff.DrawPixel(buff.Index(position), ColorToPixel(color));
    }

    void DrawRectangle(rect rec, Color color)
    {
        // todo
    }

    void DrawTriangle(triangle rec, Color color)
    {
        // todo
    }
}
