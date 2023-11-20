#pragma once

#include "standard.hpp"
#include "HelperConcepts.hpp"
#include "randomness.hpp"
#include "ExtendedAscii.hpp"

// Essentially "is this a base of 2?"
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
    constexpr const char whitespace[] = "\t\n\r\f\v";
    return str.find_first_not_of(whitespace) == string::npos;
}

// Exception thrown when a case is encountered that was not anticipated.
// This is NOT to act as a stub, but rather a "default" or "else" clause in cases thought to be exhaustive.
// This exception being thrown signifies that a new case has been made available without its pathways being complete.
class NotImplementedException :
    public exception
{
public:
    NotImplementedException() = default;
    NotImplementedException(const string& caseIdentifier)
    {
        cout << "[[Missing implementation for case \"" << caseIdentifier << "\"]]";
    }
    
    const char* what() const noexcept override
    {
        return "feature not implemented";
    }
};

constexpr bool isvowel(char ch)
{
    constexpr const char vowels[] = { 'A', 'a', 'E', 'e', 'I', 'i', 'O', 'o', 'U', 'u' };

    return find(begin(vowels), end(vowels), ch) != end(vowels);
}
