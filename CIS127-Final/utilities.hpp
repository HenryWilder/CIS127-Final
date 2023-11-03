#ifndef utilities_hpp
#define utilities_hpp

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <set>
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

constexpr bool IsExactlyOneBitSet(size_t flags);

string Prompt(const string& prompt, const vector<string>& options);
string PromptItem(const string& prompt, const map<string, int>& options);

void _RerollSurroundings(vector<string>& surroundings);

class NotImplementedException :
    public exception
{
    const char* what() const noexcept override;
};

string ChooseRandom(const vector<string>& options);
template<size_t _Size> string ChooseRandom(const array<string, _Size>& options);

bool isvowel(char ch);

// Roll a dice. The roll will succeed on average [chance] times in every [outOf] rolls.
bool DiceCheck(int chance, int outOf);

void EchoAction(const string& action, const string& target, const string& topicOrEffect);

#endif /* utilities_hpp */
