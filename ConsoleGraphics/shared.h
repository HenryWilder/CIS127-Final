#pragma once

void Sleep(size_t ms);

#include "config.h"

#define IS_USING_GRAYSCALE_ASCII (CHOSEN_COLOR_METHOD == COLOR_USE_GRAYSCALE_ASCII) // Grayscale ASCII art
#define IS_USING_ESCAPE_CODES    (CHOSEN_COLOR_METHOD == COLOR_USE_ESCAPE_CODES   ) // Color ASCII art
#define IS_USING_WINDOWS_CONTEXT (CHOSEN_COLOR_METHOD == COLOR_USE_WINDOWS_CONTEXT) // Windows pixel functions

#define IS_USING_ASCII_RAMP_STANDARD (ASCII_RAMP_STYLE == ASCII_RAMP_STANDARD) // ASCII ramp with wider range
#define IS_USING_ASCII_RAMP_SHORT    (ASCII_RAMP_STYLE == ASCII_RAMP_SHORT   ) // ASCII ramp with shorter, more convincing range
#define IS_USING_ASCII_RAMP_BLOCKS   (ASCII_RAMP_STYLE == ASCII_RAMP_BLOCKS  ) // ASCII ramp with solid blocks

// Cannot use Windows IO without Windows graphics
#if (CHOSEN_IO_METHOD == IO_USE_WINDOWS_CONTEXT) && (CHOSEN_COLOR_METHOD != COLOR_USE_WINDOWS_CONTEXT)
#undef CHOSEN_IO_METHOD
#endif

#ifndef CHOSEN_IO_METHOD
#define CHOSEN_IO_METHOD IO_USE_CONSOLE_CINCOUT
#endif

#define IS_USING_IO_CINCOUT (CHOSEN_IO_METHOD == IO_USE_CONSOLE_CINCOUT) // Console cin/cout
#define IS_USING_IO_WINDOWS (CHOSEN_IO_METHOD == IO_USE_WINDOWS_CONTEXT) // Windows.h mouse io

#if IS_USING_WINDOWS_CONTEXT
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
extern HWND console;
extern HDC dc;
#endif
