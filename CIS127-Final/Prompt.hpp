#pragma once
#include "utilities.hpp"

// Prints the elements as a markdown unordered list (preserves order).
// List starts at the current line and is ended with a newline.
template<Coutable _Ty, iterable_container_of<_Ty> _Container>
void List(const _Container& options)
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
    while (true)
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
    while (true)
    {
        string input = Prompt();
        if (_Pred(input)) return input;
    }
}

// Prompts the user for a non-empty string
string Prompt(const string& prompt);

// Prompts the user for a valid option from the provided list.
// Hidden options allow for options that are valid but aren't listed as part of the prompt (saving clutter).
template<Coutable _Ty, iterable_container_of<_Ty> _VisibleContainer, iterable_container_of<_Ty> _HiddenContainer>
string Prompt(const string& prompt, const _VisibleContainer& options, const _HiddenContainer& hiddenOptions = {})
{
    cout << prompt << '\n';
    List<_Ty, _VisibleContainer>(options);
    auto _Pred = [&](const string& input)
    {
        for (const _Ty& opt : options)
        {
            if (input == opt) return true;
        }
        for (const _Ty& opt : hiddenOptions)
        {
            if (input == opt) return true;
        }
        return false;
    };
    return PromptUntil(_Pred);
}
