#pragma once
#include <HenryLib.h>

namespace cg
{
    void Init();                                    // Enables drawing as a feature - Call once at the start of the program
    void Finished();                                // Cleans up and frees anything no longer needed

    void BeginDrawing(size_t width, size_t height); // Creates a new buffer for drawing
    void BeginDrawing(ivec2 size);                  // Creates a new buffer for drawing
    void BeginDrawing(vec2 size);                   // Creates a new buffer for drawing
    void EndDrawing();                              // Flushes the pixel buffer to the console

    void ClearBackground(Color color);              // Fills the buffer with the chosen color

    void DrawPixel(ivec2 position, Color color);    // Draws a colored pixel to the buffer
    void DrawPixel(vec2 position, Color color);     // Draws a colored pixel to the buffer

    void DrawRectangle(irect rec, Color color);     // Draws a colored rectangle to the buffer
    void DrawRectangle(rect rec, Color color);      // Draws a colored rectangle to the buffer

    void DrawTriangle(triangle rec, Color color);   // Draws a colored triangle to the buffer

    ivec2 GetRenderSize();
}
