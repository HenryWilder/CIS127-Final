#pragma once
#include <HenryLib.h>

namespace cg
{
    void Init(size_t width, size_t height);       // Creates a pixel buffer for recieving draw commands
    void Finished();                              // Cleans up and frees anything no longer needed

    void BeginDrawing();                          
    void EndDrawing();                            // Flushes the pixel buffer to the console

    void ClearBackground(Color color);            // Fills the buffer with the chosen color
    void DrawPixel(ivec2 position, Color color);  // Draws a colored pixel to the buffer
    void DrawPixel(vec2 position, Color color);   // Draws a colored pixel to the buffer
    void DrawRectangle(irect rec, Color color);   // Draws a colored rectangle to the buffer
    void DrawRectangle(rect rec, Color color);    // Draws a colored rectangle to the buffer
    void DrawTriangle(triangle rec, Color color); // Draws a colored triangle to the buffer

    ivec2 GetRenderSize();
}
