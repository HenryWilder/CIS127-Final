#include "ExtendedAscii.hpp"
#include <iostream>
#include <unordered_map>
using namespace std;

const unordered_map<wchar_t, char> EXTENDED_ASCII_MAP =
{
    { L'░', '\xB0' }, { L'▒', '\xB1' }, { L'▓', '\xB2' }, { L'│', '\xB3' }, { L'┤', '\xB4' }, { L'╡', '\xB5' }, { L'╢', '\xB6' }, { L'╖', '\xB7' },
    { L'╕', '\xB8' }, { L'╣', '\xB9' }, { L'║', '\xBA' }, { L'╗', '\xBB' }, { L'╝', '\xBC' }, { L'╜', '\xBD' }, { L'╛', '\xBE' }, { L'┐', '\xBF' },

    { L'└', '\xC0' }, { L'┴', '\xC1' }, { L'┬', '\xC2' }, { L'├', '\xC3' }, { L'─', '\xC4' }, { L'┼', '\xC5' }, { L'╞', '\xC6' }, { L'╟', '\xC7' },
    { L'╚', '\xC8' }, { L'╔', '\xC9' }, { L'╩', '\xCA' }, { L'╦', '\xCB' }, { L'╠', '\xCC' }, { L'═', '\xCD' }, { L'╬', '\xCE' }, { L'╧', '\xCF' },

    { L'╨', '\xD0' }, { L'╤', '\xD1' }, { L'╥', '\xD2' }, { L'╙', '\xD3' }, { L'╘', '\xD4' }, { L'╒', '\xD5' }, { L'╓', '\xD6' }, { L'╫', '\xD7' },
    { L'╪', '\xD8' }, { L'┘', '\xD9' }, { L'┌', '\xDA' }, { L'█', '\xDB' }, { L'▄', '\xDC' }, { L'▌', '\xDD' }, { L'▐', '\xDE' }, { L'▀', '\xDF' },
};

char ExtendedAscii(wchar_t ch)
{
    auto it = EXTENDED_ASCII_MAP.find(ch);
    return it != EXTENDED_ASCII_MAP.end() ? it->second : (char)ch;
}

ostream& operator<<(ostream& stream, wchar_t ch)
{
    return stream << ExtendedAscii(ch);
}

ostream& operator<<(ostream& stream, const wchar_t* str)
{
    for (; *str; ++str)
    {
        stream << ExtendedAscii(*str);
    }
    return stream;
}
