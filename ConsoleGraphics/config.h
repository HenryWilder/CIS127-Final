#pragma once
#include "config_options.h"

// The whole reason I made this is so you can recompile the dll with
// a different configuration depending on what works for your system.

// How pixels are drawn
// Options:
// - COLOR_USE_GRAYSCALE_ASCII
// - COLOR_USE_ESCAPE_CODES
// - COLOR_USE_WINDOWS_CONTEXT
#define CHOSEN_COLOR_METHOD \
    COLOR_USE_WINDOWS_CONTEXT

// When using Windows.h, whether to fill pixel-by-pixel instead of entire rectangles.
// This is slower, but gives a more animated appearance and fixes certain rare graphical bugs.
// 
// Options:
// - 0 (false)
// - 1 (true)
#define USE_SLOW_FILL 0

// When not using Windows.h, chooses which style of ASCII-art to use.
// 
// Options:
// - ASCII_RAMP_STANDARD
// - ASCII_RAMP_SHORT
// - ASCII_RAMP_BLOCKS
#define ASCII_RAMP_STYLE \
    ASCII_RAMP_BLOCKS
