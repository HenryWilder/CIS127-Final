#pragma once

//
// Draw method options
//

#define COLOR_USE_GRAYSCALE_ASCII 0 // DLL should use grayscale ASCII art
#define COLOR_USE_ESCAPE_CODES    1 // DLL should use color escape codes
#define COLOR_USE_WINDOWS_CONTEXT 2 // DLL should use Windows.h pixel functions

//
// ASCII ramp options
//

#define ASCII_RAMP_STANDARD 0 // DLL should use an ASCII ramp with a wide range
#define ASCII_RAMP_SHORT    1 // DLL should use an ASCII ramp with a short, but convincing range
#define ASCII_RAMP_BLOCKS   2 // DLL should use an ASCII ramp with block characters

//
// Input/output method
//

#define IO_USE_CONSOLE_CINCOUT 0 // DLL should use cin/cout/printf for 
#define IO_USE_WINDOWS_CONTEXT 1 // DLL should use Windows.h mouse input
