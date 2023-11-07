#pragma once
#include "utilities.hpp"

constexpr char listItemBullet = '-';

template<Coutable _Ty>
void ListItem(ostream& stream, _Ty item)
{
    stream << listItemBullet << ' ' << item << '\n';
}

template<Coutable _Key, Coutable _Val>
void ListPairItem(ostream& stream, pair<_Key, _Val> item, const char* separator = ": ")
{
    stream << listItemBullet << ' ' << item.first << separator << item.second << '\n';
}

// Prints the elements as a markdown unordered list (preserves order).
// List starts at the current line and is ended with a newline.
template<Coutable _Ty, iterable_container_of<_Ty> _Container>
void List(ostream& stream, const _Container& options)
{
    for (const _Ty& opt : options)
        ListItem<_Ty>(stream, opt);
}
template<Coutable _Ty, iterable_container_of<_Ty> _Container>
void List(const _Container& options)
{
    List(cout, options);
}

template<Coutable _Key, Coutable _Val>
void List(ostream& stream, const map<_Key, _Val>& options, const char* separator = ": ")
{
    for (const auto& opt : options)
        ListPairItem<_Key, _Val>(stream, opt, separator);
}
template<Coutable _Key, Coutable _Val>
void List(const map<_Key, _Val>& options, const char* separator = ": ")
{
    List(cout, options, separator);
}

template<Coutable _Key, class _Val>
void ListKeys(ostream& stream, const map<_Key, _Val>& options)
{
    for (const auto& [key, _] : options)
        ListItem(stream, key);
}
template<Coutable _Key, class _Val>
void ListKeys(const map<_Key, _Val>& options)
{
    ListKeys(cout, options);
}

// Awaits a non-empty (and not-exclusively-whitespace) string from the user, prefixing each attempt with "> ".
string Prompt();

// Prompts the user for a non-empty (and not-exclusively-whitespace) string, prefixing each attempt with "> ".
string Prompt(const string& prompt);

// Prompts the user for a valid option from the provided list.
template<Coutable _Ty = string, iterable_container_of<_Ty> _VisibleContainer = initializer_list<_Ty>>
_Ty Prompt(const string& prompt, const _VisibleContainer& options)
{
    cout << prompt << '\n';
    List<_Ty, _VisibleContainer>(options);
    while (true)
    {
        string input = Prompt();
        auto it = find(begin(options), end(options), input);
        if (it != end(options)) return *it;
    }
}

// Prompts the user for a valid option from the provided list.
// Hidden options allow for options that are valid but aren't listed as part of the prompt (saving clutter).
template<Coutable _Ty = string, iterable_container_of<_Ty> _VisibleContainer = initializer_list<_Ty>, iterable_container_of<_Ty> _HiddenContainer = initializer_list<_Ty>>
_Ty Prompt(const string& prompt, const _VisibleContainer& options, const _HiddenContainer& hiddenOptions)
{
    cout << prompt << '\n';
    List<_Ty, _VisibleContainer>(options);
    while (true)
    {
        string input = Prompt();
        auto visIt = find(begin(options), end(options), input);
        if (visIt != end(options)) return *visIt;
        auto hidIt = find(begin(hiddenOptions), end(hiddenOptions), input);
        if (hidIt != end(hiddenOptions)) return *hidIt;
    }
}

// Prompts the user for a valid option from the provided list.
// Hidden options allow for options that are valid but aren't listed as part of the prompt (saving clutter).
template<Coutable _Key = string, class _Val>
_Key PromptKey(const string& prompt, const map<_Key, _Val>& options)
{
    cout << prompt << '\n';
    ListKeys<_Key>(options);
    while (true)
    {
        string input = Prompt();
        for (const auto& [key, _] : options)
        {
            if (input == key) return key;
        }
    }
}
