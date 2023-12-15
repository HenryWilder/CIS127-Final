#pragma once
#include "utilities/pch/utilities.hpp"

/**
 * @brief Alternative to cout which prints inside a box
 * @note Colors automatically reset at the end of a line. Using `COLOR_RESET`
 *       at the line end is redundant and may mess with the box printing.
 */
extern stringstream echo;
constexpr size_t echoBoxInsideWidth = 100;

// Prints the stream to cout (inside a box) and clears the stream.
void FlushEcho( );

#define COLOR_BLACK   "\x1B[30m"
#define COLOR_RED     "\x1B[31m"
#define COLOR_GREEN   "\x1B[32m"
#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_BLUE    "\x1B[34m"
#define COLOR_MAGENTA "\x1B[35m"
#define COLOR_CYAN    "\x1B[36m"
#define COLOR_WHITE   "\x1B[37m"
#define COLOR_GRAY    "\x1B[90m"
#define COLOR_RESET   "\x1B[0m"

#define COLOR_MESSAGE COLOR_CYAN
