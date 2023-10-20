#pragma once
#include "custom_concepts.h"
#include <string>
using std::string;

template<numeric _Ty>
_Ty StringTo(const string& str);

// Converts string to char as an 8-bit integer, not a character.
template<>
bool StringTo<bool>(const string& str)
{
    if (str == "true" || str == "1")
    {
        return true;
    }

    if (str == "false" || str == "0")
    {
        return false;
    }
}

// Converts string to char as an 8-bit integer, not a character.
template<>
char StringTo<char>(const string& str)
{
    return (char)std::stoi(str);
}

// Converts string to char as an 8-bit integer, not a character.
template<>
unsigned char StringTo<unsigned char>(const string& str)
{
    return (unsigned char)std::stoi(str);
}

template<>
short StringTo<short>(const string& str)
{
    return (short)std::stoi(str);
}

template<>
unsigned short StringTo<unsigned short>(const string& str)
{
    return (unsigned short)std::stoi(str);
}

template<>
int StringTo<int>(const string& str)
{
    return std::stoi(str);
}

template<>
unsigned int StringTo<unsigned int>(const string& str)
{
    return (unsigned int)std::stoul(str);
}

template<>
long StringTo<long>(const string& str)
{
    return std::stol(str);
}

template<>
long long StringTo<long long>(const string& str)
{
    return std::stoll(str);
}

template<>
unsigned long StringTo<unsigned long>(const string& str)
{
    return std::stoul(str);
}

template<>
unsigned long long StringTo<unsigned long long>(const string& str)
{
    return std::stoull(str);
}

template<>
float StringTo<float>(const string& str)
{
    return std::stof(str);
}

template<>
double StringTo<double>(const string& str)
{
    return std::stod(str);
}

template<>
long double StringTo<long double>(const string& str)
{
    return std::stold(str);
}
