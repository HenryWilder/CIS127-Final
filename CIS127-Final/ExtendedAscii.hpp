#pragma once
#include <iostream>

std::ostream& operator<<(std::ostream& stream, wchar_t ch);
std::ostream& operator<<(std::ostream& stream, const wchar_t* str);
