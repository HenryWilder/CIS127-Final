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
#include <utility>
#include <type_traits>
#include <iterator>
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

template<class _Ty>
concept Coutable = requires(_Ty x)
{
    cout << x;
};

template<class _Container>
concept iterable_container = requires(_Container _Cont)
{
    begin(_Cont);
      end(_Cont);
    {  begin(_Cont) } -> forward_iterator;
};

template<class _Container, class _Valty>
concept iterable_container_of = iterable_container<_Container> && requires(_Container _Cont)
{
    { *begin(_Cont) } -> convertible_to<_Valty>;
};

template<class _Container, class _Valty>
concept integral_indexable_container_of = requires(_Container _Cont, size_t index)
{
    { _Cont[index] } -> convertible_to<_Valty>;
    { _Cont.size() } -> same_as<size_t>;
};

template<class _Container>
concept integral_indexable_container = integral_indexable_container_of<_Container, typename _Container::value_type>;


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

bool isvowel(char ch);

void EchoAction(const string& action, const string& target, const string& topicOrEffect);

#endif /* utilities_hpp */
