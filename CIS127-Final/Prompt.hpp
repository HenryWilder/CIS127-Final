#pragma once
#include "utilities.hpp"

extern const char* streamListItemPrefix;
extern const char* streamListItemSeparator;
extern const char* streamListItemMemberSeparator;

template<class _Ty1, class... _TyN>
void ListItem(ostream& stream, const _Ty1& member1, const _TyN&... memberN)
{
      stream << streamListItemPrefix          << member1;
    ((stream << streamListItemMemberSeparator << memberN), ...);
      stream << streamListItemSeparator;
}

template<input_iterator _InIt>
void List(ostream& stream, _InIt begin, _InIt end)
{
    for (; begin != end; ++begin)
        ListItem(stream, *begin);
}

template<input_iterator _InIt>
void List(ostream& stream, _InIt begin, _InIt end)
    requires(specialization_of<remove_const_t<remove_reference_t<decltype(*begin)>>, pair>)
{
    for (; begin != end; ++begin)
        ListItem(stream, begin->first, begin->second);
}

// Prints the elements as a markdown unordered list (preserves order).
// List starts at the current line and is ended with a newline.
template<iterable _Container>
void List(ostream& stream, const _Container&& options)
{
    List(stream, begin(options), end(options));
}

template<iterable _Container>
void List(ostream& stream, const _Container& options)
{
    List(stream, begin(options), end(options));
}

template<iterable _Container>
void ListKeys(ostream& stream, const _Container& options)
{
    for (const auto& [key, _] : options)
        ListItem(stream, key);
}

// Awaits a non-empty (and not-exclusively-whitespace) string from the user, prefixing each attempt with "> ".
string Prompt();

// Prompts the user for a non-empty (and not-exclusively-whitespace) string, prefixing each attempt with "> ".
string Prompt(const string& prompt);

// Prompts the user for a valid option from the provided list.
template<iterable _Container = initializer_list<const char*>>
auto PromptOption(const string& prompt, const _Container& options)
{
    cout << prompt << '\n';
    List(cout, options);
    while (true)
    {
        string input = Prompt();

        auto it = find(begin(options), end(options), input);
        if (it != end(options)) return *it;
    }
}

template<class _Container1, class _Container2>
concept interchangeable_value_type = iterable<_Container1> && iterable<_Container2> && requires(_Container1 _Cont1, _Container2 _Cont2, bool testBool)
{
    (testBool ? (*begin(_Cont1)) : (*begin(_Cont2)));
};

// Prompts the user for a valid option from the provided list.
// Hidden options allow for options that are valid but aren't listed as part of the prompt (saving clutter).
template<iterable _VisibleContainer = initializer_list<const char*>, iterable _HiddenContainer = initializer_list<const char*>>
auto PromptOptionWithHidden(const string& prompt, const _VisibleContainer& options, const _HiddenContainer& hiddenOptions)
    requires(interchangeable_value_type<_VisibleContainer, _HiddenContainer>)
{
    cout << prompt << '\n';
    List(cout, options);
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
template<iterable _Container>
auto PromptKey(const string& prompt, const _Container& options)
{
    cout << prompt << '\n';
    ListKeys(cout, options);
    while (true)
    {
        string input = Prompt();
        for (const auto& [key, _] : options)
        {
            if (input == key) return key;
        }
    }
}
