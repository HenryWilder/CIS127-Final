#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <concepts>
#include <variant>
#include <algorithm>
using std::find;
using std::find_if;
using std::cin;
using std::cout;
using std::getline;
using std::tuple;
using std::get;
using std::string;
using std::vector;
using std::unordered_map;
using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;
using std::same_as;
using std::initializer_list;
using std::pair;
using std::variant;
using std::exception;
using byte = unsigned char;

constexpr bool USE_COLORED_TEXT = true;

struct ColoredText
{
    string text;
    byte r, g, b;
};

ColoredText Colored(const string& text, _In_range_(0, 255) byte r, _In_range_(0, 255) byte g, _In_range_(0, 255) byte b);
ColoredText Colored(const string& text, _In_range_(0, 0xFFFFFF) int hex);

template<int _Hex>
ColoredText Colored(const string& text)
    requires(0 <= _Hex && _Hex <= 0xFFFFFF)
{
    return {
        text,
        (byte)(_Hex >> 020),
        (byte)(_Hex >> 010),
        (byte)(_Hex >> 000)
    };
}

ostream& operator<<(ostream& stream, const ColoredText& text);

constexpr int SKYBLUE = 0x0080FF;
constexpr int WHITE   = 0xFFFFFF;
constexpr int RED     = 0xFF0000;
constexpr int GREEN   = 0x00FF00;
constexpr int BLUE    = 0x0000FF;
constexpr int GOLD    = 0xFFD700;


string PromptString(const string& query);

// Returns an iterator so that both value and index can be known without redundantly searching the option list
vector<string>::const_iterator Prompt(const string& query, const vector<string>& options);

// Returns a pointer which can be compared to the array to get index
const string* Prompt(const string& query, const string* options, size_t numOptions);

template<size_t _NumElements>
const string* Prompt(const string& query, const string(&options)[_NumElements])
{
    return Prompt(query, options, _NumElements);
}
