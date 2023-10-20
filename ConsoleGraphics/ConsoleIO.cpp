#include "ConsoleIO.h"
#include "ConsoleGraphics.h"
#include "shared.h"
#include <algorithm>
#include <type_traits>
#include <concepts>

#if IS_USING_IO_CINCOUT

// todo

#elif IS_USING_IO_WINDOWS

#include <Windows.h>

HWND console;
HDC dc;

ivec2 GetMousePos()
{
    POINT p;
    if (GetCursorPos(&p) && ScreenToClient(console, &p))
    {
        return ivec2((int)p.x, (int)p.y);
    }
    return negative<ivec2>;
}

enum KeyState
{
    VK_KEYSTATE_DOWN = 0x8000,
};

#else
#error Must select a valid IO option
#endif

namespace cio
{
    string PromptText(string question)
    {
        cout << question;
        string response;
        getline(cin, response);
        return response;
    }

    size_t Prompt(const vector<string>& options)
    {
        const size_t numOptions = options.size();
        size_t index = options.size();
#if IS_USING_IO_CINCOUT

        for (const string& opt : options)
        {
            cout << opt << '\n';
        }

        while (index == numOptions)
        {
            string input;
            cin >> input;
            index = FindIndex(options, input);
        }

#elif IS_USING_IO_WINDOWS

        console = GetConsoleWindow();
        dc = GetWindowDC(console);

        ivec2 pt = GetMousePos();

        ReleaseDC(console, dc);

#endif
        return index;
    }
}
