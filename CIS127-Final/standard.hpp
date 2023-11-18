#pragma once

#include <string>
#include <format>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>

#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>

#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <stack>
#include <queue>

#include <span>

#include <cassert>
#include <random>
#include <algorithm>
#include <concepts>
#include <utility>
#include <type_traits>
#include <iterator>
#include <functional>
#include <regex>

#include <tuple>
#include <variant>
#include <any>

using namespace std;
namespace fs = filesystem;

void PrettyError(const string& errorType, const string& file, const string& function, size_t line, const string& msg);

// "do {} while(false)" Makes the macro syntactically correct to follow with a semicolon
#if _DEBUG
#define dynamic_assert(cond, msg)\
    do {\
        if (!(cond)) {\
            PrettyError("failed assertion `" #cond "`", __FILE__, __FUNCSIG__, (size_t)__LINE__, (msg));\
            std::terminate(); \
        }\
        __assume(cond);\
    } while (false)
#else
#define dynamic_assert(cond, msg) do {} while (false)
#endif
