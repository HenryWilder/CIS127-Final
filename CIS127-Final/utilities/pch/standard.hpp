#pragma once

// This file acts as my pch

#pragma warning(push)
// kinda wonder why the standard library is causing this warning.
// I've checked, but the argument being converted isn't a variable within my control.
// It appears that the warning is being caused by the very instantiation of the function, and not as a result of any abuse.
#pragma warning(disable:4365)

#include <sal.h>

#include <string>
#include <string_view>
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
#include <memory>

#include <tuple>
#include <variant>
#include <any>

#pragma warning(pop)

using namespace std;
namespace fs = filesystem;

#if _DEBUG
void PrettyError(const string &errorType, const string &file, const string &function, size_t line, const string &msg);

// Custom runtime assertion with more readable formatting
#define dynamic_assert(cond, msg)\
    do {\
        if (!(cond)) {\
            PrettyError("failed assertion `" #cond "`", __FILE__, __FUNCSIG__, (size_t)__LINE__, (msg));\
            std::terminate(); \
        }\
        __assume(cond);\
    } while (false)
#else
#define PrettyError(errorType, file, function, line, msg) static_assert(false, "Should not be using PrettyError in release build.")
#define dynamic_assert(cond, msg) do {} while (false)
#endif

class failed_runtime_check :
    public runtime_error
{
public:
    explicit failed_runtime_check(const string& msg) :
        runtime_error(msg) {}

    explicit failed_runtime_check(const string& context, const string& msg) :
        runtime_error(context + msg) {}

    string contextAddedMsg(const string& context, const string& msg)
    {
        return context + ":\n  " + regex_replace(msg, regex("\\n"), "\n  ");
    }
};

// Similar to dynamic_assert, but ships with release build.
// Intended to provide non-bug failure information to the user.
// Make sure there is a try-catch block somewhere above this.
// ---
// Errors should be capitalized. Multi-line messages are allowed.
// End sentences with a period.
#define runtime_check(cond, location, err, suggestions)\
    do {\
        if (!(cond)) {\
            throw new failed_runtime_check((err));\
        }\
    } while (false)

// Adds additional context to failed runtime_checks. Includes own catch() block.
// Can be nested as many times as needed to create detailed traceback.
// ---
// Context should be formatted such that "{context}:\n  {error}" is grammatically correct.
// Example:
// ```
// While reading save.txt:
//   At line [number] of save.txt
// ```
// ---
// Errors should be capitalized. Multi-line messages & contexts are allowed.
// Last line of context will end with a colon followed by a newline, and following message will be indented.
#define catch_and_rethrow_runtimecheck(context)\
    catch (runtime_error* err) { throw new failed_runtime_check((context), (err->what())); }
