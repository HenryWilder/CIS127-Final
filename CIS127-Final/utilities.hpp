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
using namespace std;

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

// Prompts the user for a valid option from the provided list.
// Hidden options allow for options that are valid but aren't listed as part of the prompt (saving clutter).
string Prompt(const string& prompt, const vector<string>& options, const vector<string>& hiddenOptions = {});

// Prompts the user for a non-empty string
string Prompt(const string& prompt);

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

string ChooseRandom(const vector<string>& options);
string ChooseRandom(vector<string>::const_iterator optionsBegin, vector<string>::const_iterator optionsEnd);

bool isvowel(char ch);

// Roll a dice. The roll will succeed on average [chance] times in every [outOf] rolls.
bool DiceCheck(int chance, int outOf);

void EchoAction(const string& action, const string& target, const string& topicOrEffect);

#endif /* utilities_hpp */
