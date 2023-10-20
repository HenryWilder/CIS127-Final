#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <concepts>
#include <variant>
#include <algorithm>
using std::find;
using std::find_if;
using std::cin;
using std::cout;
using std::getline;
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

ostream& operator<<(ostream& stream, const ColoredText& text);

constexpr int SKYBLUE = 0x0080FF;
constexpr int WHITE   = 0xFFFFFF;
constexpr int RED     = 0xFF0000;
constexpr int GREEN   = 0x00FF00;
constexpr int BLUE    = 0x0000FF;
constexpr int GOLD    = 0xFFD700;


string PromptString(const string& query);
vector<string>::const_iterator Prompt(const string& query, const vector<string>& options);
