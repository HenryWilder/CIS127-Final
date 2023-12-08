#pragma once
#include "utilities/pch/utilities.hpp"

// Alternative to cout which prints inside a box
extern stringstream echo;
constexpr size_t echoBoxInsideWidth = 100;

// Prints the stream to cout (inside a box) and clears the stream.
void FlushEcho( );
