#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <concepts>
#include <variant>
#include <optional>
#include <algorithm>
#include <functional>
#include <format>
#include "stl_overloads.h"
#include "IVec2.h"
using std::format;
using std::find;
using std::find_if;
using std::cin;
using std::cout;
using std::getline;
using std::tuple;
using std::get;
using std::string;
using std::wstring;
using std::to_wstring;
using std::basic_string;
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
using std::optional;
using std::hash;
using std::nullopt;
using std::exception;
using std::min;
using std::max;
using byte = unsigned char;
using cstring_t = const char*;
using template_cstring_t = const char[];

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

struct PromptOption
{
    PromptOption() = default;

    constexpr PromptOption(string input) :
        input(input), description(nullopt) {}

    constexpr PromptOption(string input, string description) :
        input(input), description(description) {}

    string input;
    optional<string> description;
};
using PromptOptionList = vector<PromptOption>;
using PromptOptionIter = vector<PromptOption>::const_iterator;

// Returns an iterator so that both value and index can be known without redundantly searching the option list
PromptOptionIter Prompt(const string& query, const PromptOptionList& options);

ostream& operator<<(ostream& stream, IVec2 v);
istream& operator>>(istream& stream, IVec2& v);

template<class _Ty>
concept wstringable_convertable = requires(_Ty value)
{
    { to_wstring(value) } -> same_as<wstring>;
};

template<wstringable_convertable _Ty>
wstring operator+(const wstring a, _Ty b)
{
    return a + to_wstring(b);
}

inline wstring operator+(const wstring a, const string b)
{
    return a + wstring(b.begin(), b.end());
}

inline wstring operator+(const wstring a, const char* b)
{
    return a + string(b);
}

#define assert(cond, msg) _ASSERT_EXPR((cond), (std::wstring(L"") + msg).c_str())
