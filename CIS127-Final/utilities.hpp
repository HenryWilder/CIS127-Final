#ifndef utilities_hpp
#define utilities_hpp

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
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

// Prompts the user for a valid option from the provided list
string Prompt(const string& prompt, const initializer_list<string>& options);

// Prompts the user for a non-empty string
string Prompt(const string& prompt);

/// Exception thrown when a case is encountered that was not anticipated.
/// This is NOT to act as a stub, but rather a "default" or "else" clause in cases thought to be exhaustive.
/// This exception being thrown signifies that a new case has been made available
/// without its pathways being complete.
///
/// # Examples
/// ## DO
/// ### ex. a1
/// ```
/// enum MyEnum
/// {
///     MYENUM_BIG,
///     MYENUM_LIL,
/// }
///
/// switch (myEnum)
/// {
///    case MYENUM_BIG: /* ... */ break;
///    case MYENUM_LIL: /* ... */ break;
///    default: throw new NotImplementedException("myEnum was " + to_string(myEnum));
/// }
/// ```
/// By throwing a `NotImplementedException` in the default case, the tester will know that the above switch statement
/// expects, but lacks, a case for handling `MYENUM_MID` if that is added to MyEnum in the future, but is forgotten in the switch statement.
///
/// ### ex. a2
/// ```
/// if (myFruit == "apple")
/// {
///     MakeAppleJuice();
/// }
/// else if (myFruit == "lemon")
/// {
///     MakeLemonade();
/// }
/// else
/// {
///     throw new NotImplementedException(myFruit);
/// }
/// ```
///
/// ## DON'T
/// ### ex. b1
/// ```
/// if (myFruit == "apple")
/// {
///     MakeAppleJuice();
/// }
/// else if (myFruit == "orange")
/// {
///     MakeOrangeJuice();
/// }
/// else if (myFruit == "banana")
/// {
///     throw new NotImplementedException("banana");
/// }
/// ```
/// The if/else tree above has "banana" as an anticipated case. Rather than throwing a `NotImplementedException` here,
/// it would be better to write a stub (like printing "todo"). The "banana" case technically *is* implemented, it just hasn't been properly defined yet.
///
class NotImplementedException :
    public exception
{
public:
    NotImplementedException(const string& caseIdentifier)
    {
        cout << "[[Missing implementation for case \"" << caseIdentifier << "\"]]";
    }
    
    const char* what() const noexcept override;
};

string ChooseRandom(const vector<string>& options);
string ChooseRandom(const vector<string>& options);

bool isvowel(char ch);

// Roll a dice. The roll will succeed on average [chance] times in every [outOf] rolls.
bool DiceCheck(int chance, int outOf);

void EchoAction(const string& action, const string& target, const string& topicOrEffect);

#endif /* utilities_hpp */
