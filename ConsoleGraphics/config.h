#pragma once
#include "config_options.h"

// The whole reason I made this is so you can recompile the dll with
// a different configuration depending on what works for your system.


//
// Graphics options
//

// How pixels are drawn
// 
// Options:
// - COLOR_USE_GRAYSCALE_ASCII  | Black and white, but universally portable.
// - COLOR_USE_ESCAPE_CODES     | Has colors, but not portable.
// - COLOR_USE_WINDOWS_CONTEXT  | The best-looking, but requires Windows
#define CHOSEN_COLOR_METHOD \
    COLOR_USE_WINDOWS_CONTEXT

#if (CHOSEN_COLOR_METHOD == COLOR_USE_GRAYSCALE_ASCII)

// When not using Windows.h, chooses which style of ASCII-art to use.
// 
// Options:
// - ASCII_RAMP_STANDARD | Gives a wider range of shades (72 total), but shades are unevenly spaced out.
// - ASCII_RAMP_SHORT    | Evenly spaced shades, but only has about 10 shades.
// - ASCII_RAMP_BLOCKS   | Space-filling and very evenly spaced shades, but only has 5 shades and is very blocky.
#define ASCII_RAMP_STYLE \
    ASCII_RAMP_STANDARD

#endif


//
// IO options
//

#if (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT)

// How inputs are provided
//
// Options:
// - IO_USE_CONSOLE_CINCOUT | Options are selected through user text-based input
// - IO_USE_WINDOWS_CONTEXT | Options are selected by clicking with the mouse (must already use Windows graphics)
#define CHOSEN_IO_METHOD \
    IO_USE_WINDOWS_CONTEXT

#endif
