#ifndef utilities_hpp
#define utilities_hpp

#include <iostream>
#include <string>
#include <format>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cassert>
#include <concepts>
using namespace std;

// I prefer Rust's syntax for this.
#define loop while(true)

// Interface for classes that can be mutated but not reassigned
// Useful for objects used exclusively as properties
class NotCopyable
{
protected:
    // Default ctor/dtor
    
    constexpr NotCopyable() = default;
    ~NotCopyable() = default;
    
    // Delete assignment operators
    
    NotCopyable(const NotCopyable&) = delete;
    NotCopyable& operator=(const NotCopyable&) = delete;
};

constexpr bool IsExactlyOneBitSet(size_t flags)
{
    bool isSetBitEncountered = false;
    while (flags)
    {
        if (flags & 1)
        {
            if (isSetBitEncountered)
            {
                return false;
            }
            isSetBitEncountered = true;
        }
        flags >>= 1;
    }
    return isSetBitEncountered;
}

constexpr string Trim(const string& str)
{
    const char* whitespace = "\t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(whitespace) + 1;
    return str.substr(start, end - start);
}

constexpr bool IsEmptyOrWhitespace(const string& str)
{
    return str.empty() || all_of(str.begin(), str.end(), isspace);
}

template<class _Ty>
concept Coutable = requires(_Ty x) { cout << x; };

// Prints the elements as a markdown unordered list (preserves order).
// List starts at the current line and is ended with a newline.
template<Coutable _Ty>
void List(const vector<_Ty>& options)
{
    for (const _Ty& opt : options)
    {
        cout << "- " << opt << '\n';
    }
}

// Awaits a non-empty (and not-exclusively-whitespace) string from the user, prefixing each attempt with "> ".
string Prompt();

// Awaits a string from the user which succeeds the provided predicate.
// Optimized for static functions.
template<bool(*_Pred)(const string&)>
string PromptUntil()
{
    loop
    {
        string input = Prompt();
        if (_Pred(input)) return input;
    }
}

// Awaits a string from the user which succeeds the provided predicate.
// Allows lambdas.
template<predicate<const string&> _FTy>
string PromptUntil(_FTy _Pred)
{
    loop
    {
        string input = Prompt();
        if (_Pred(input)) return input;
    }
}

// Prompts the user for a non-empty string
string Prompt(const string& prompt);

// Prompts the user for a valid option from the provided list.
// Hidden options allow for options that are valid but aren't listed as part of the prompt (saving clutter).
string Prompt(const string& prompt, const vector<string>& options, const vector<string>& hiddenOptions = {});


// Exception thrown when a case is encountered that was not anticipated.
// This is NOT to act as a stub, but rather a "default" or "else" clause in cases thought to be exhaustive.
// This exception being thrown signifies that a new case has been made available without its pathways being complete.
class NotImplementedException :
    public exception
{
public:
    NotImplementedException() = default;
    NotImplementedException(const string& caseIdentifier);
    
    const char* what() const noexcept override;
};

template<class _Ty>
_Ty ChooseRandom(
    typename vector<_Ty>::const_iterator optionsBegin,
    typename vector<_Ty>::const_iterator optionsEnd)
{
    return *(optionsBegin + (rand() % (optionsEnd - optionsBegin)));
}

template<class _Ty>
_Ty ChooseRandom(const vector<_Ty>& options)
{
    return options.at(rand() % options.size());
}

template<class _Ty>
_Ty ChooseRandom(const _Ty* optionsBegin, const _Ty* optionsEnd)
{
    return *(optionsBegin + (rand() % (optionsEnd - optionsBegin)));
}

bool isvowel(char ch);

// Roll a dice. The roll will succeed on average [chance] times in every [outOf] rolls.
bool DiceCheck(int chance, int outOf);

// Epsilon is 0.001.
// Chance less than epsilon will always fail.
// Chance greater than [1 minus epsilon] will always succeed.
bool Chance(float chance01);

// Flip a coin. Succeed if heads; fail if tails.
// Succeeds 50% of the time.
bool CoinFlip();

// Flip two coins. Succeed if either is heads; fail if both are tails.
// Succeeds 75% of the time.
bool AdvantagedCoinFlip();

// Flip two coins. Succeed if both are heads; fail if either is tails.
// Succeeds 25% of the time.
bool DisadvantagedCoinFlip();

void EchoAction(const string& action, const string& target, const string& topicOrEffect);

#endif /* utilities_hpp */
