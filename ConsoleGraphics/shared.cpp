#include "shared.h"
#include <thread>

void Sleep(size_t ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

#if IS_USING_WINDOWS_CONTEXT
HWND console;
HDC dc;
#endif
