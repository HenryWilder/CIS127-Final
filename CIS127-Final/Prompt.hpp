#pragma once
#include "utilities.hpp"

template<ostreamable _Ty>
size_t MeasureStreamable(_Ty item)
{
    stringstream streamSimulator;
    streamSimulator << item;
    streamSimulator.seekg(0, ios::end);
    size_t size = streamSimulator.tellg();
    return size;
}

template<input_iterator _It>
using IteratorOStreamer = void(*)(ostream& o, const _It& it);

struct ListStyle
{
    const char* start;
    const char* prefix;
    const char* memberSeparator;
    const char* suffix;
    const char* separator;
    const char* finalSeparator = 0; // 0 represents "use separator"
    const char* end;
    bool spaceMembersEvenly = false;
};

class StreamList
{
private:
    StreamList() :
        style({ MarkdownUnorderedList }) {}

    static StreamList& Get()
    {
        static StreamList singleton;
        return singleton;
    }

    const ListStyle& GetCurrentStyle() const
    {
        return style.top();
    }

    template<input_iterator _InIt, IteratorOStreamer<_InIt> _DerefStreamer1, IteratorOStreamer<_InIt>... _DerefStreamers>
    void ListItem(ostream& stream, const streamsize (&widths)[sizeof...(_DerefStreamers) + 1ull], const _InIt& it) const
    {
        const ListStyle& currentStyle = GetCurrentStyle();
        stream << currentStyle.prefix;
        stream << setw(widths[0]);
        _DerefStreamer1(stream, it);
        size_t i = 1;
        (((stream << currentStyle.memberSeparator << setw(widths[i++])),  _DerefStreamers(stream, it)), ...);
        stream << currentStyle.suffix;
    }

    template<input_iterator _InIt, IteratorOStreamer<_InIt> _StreamDeref>
    streamsize MeasureEvenSpacing(_InIt beginIt, _InIt endIt) const
    {
        if (GetCurrentStyle().spaceMembersEvenly)
        {
            streamsize width = 0;
            for (_InIt it = beginIt; it != endIt; ++it)
            {
                stringstream streamSimulator;
                _StreamDeref(streamSimulator, it);
                streamSimulator.seekg(0, ios::end);
                streampos itWidth = streamSimulator.tellg();
                width = max<streamsize>(width, itWidth);
            }
            return width;
        }
        return 0;
    }

    template<input_iterator _InIt>
    static _InIt Next(const _InIt it)
    {
        _InIt next = it;
        return ++next;
    }

    template<input_iterator _InIt, IteratorOStreamer<_InIt> _DerefStreamer1, IteratorOStreamer<_InIt>... _DerefStreamers>
    void _List(ostream& stream, _InIt beginIt, _InIt endIt) const
    {
        const ListStyle& currentStyle = GetCurrentStyle();

        bool isUsingFinalSeparator = currentStyle.finalSeparator != 0;

        streamsize widths[] = {
            MeasureEvenSpacing<_InIt, _DerefStreamer1>(beginIt, endIt),
            MeasureEvenSpacing<_InIt, _DerefStreamers>(beginIt, endIt)...
        };

        stream << currentStyle.start;
        ListItem<_InIt, _DerefStreamer1, _DerefStreamers...>(stream, widths, beginIt);
        if (isUsingFinalSeparator)
        {
            for (++beginIt; beginIt != endIt; ++beginIt)
            {
                const char* sep = Next(beginIt) == endIt ? currentStyle.finalSeparator : currentStyle.separator;
                stream << sep;
                ListItem<_InIt, _DerefStreamer1, _DerefStreamers...>(stream, widths, beginIt);
            }
        }
        else
        {
            for (++beginIt; beginIt != endIt; ++beginIt)
            {
                stream << currentStyle.separator;
                ListItem<_InIt, _DerefStreamer1, _DerefStreamers...>(stream, widths, beginIt);
            }
        }
        stream << currentStyle.end;
    }

public:
    static void Push(ListStyle style)
    {
        Get().style.push(style);
    }
    static void Pop()
    {
        Get().style.pop();
    }

    template<input_iterator _InIt, IteratorOStreamer<_InIt>... _DerefStreamers>
    static void List(ostream& stream, _InIt beginIt, _InIt endIt)
    {
        Get()._List<_InIt, _DerefStreamers...>(stream, beginIt, endIt);
    }

    static constexpr ListStyle JSONObjectList{ "[\n", "  { ", ": ", " }", ",\n", 0, "\n]\n", true };
    static constexpr ListStyle JSONValueList{ "[\n", "  ", "", "", ",\n", 0, "\n]\n", false };
    static constexpr ListStyle JSONInlineObjectList{ "[ ", "  { ", ": ", " }", ", ", 0, " ]", false };
    static constexpr ListStyle JSONInlineValueList{ "[ ", "  ", "", "", ", ", 0, " ]", false };
    static constexpr ListStyle MarkdownUnorderedList{ "", " - ", "", "", "\n", 0, "\n", false };

private:
    stack<ListStyle> style;
};

template<input_iterator _InIt>
constexpr void ItOStreamer_Deref(ostream& stream, const _InIt& it)
{
    stream << (*it);
}

template<input_iterator _InIt>
constexpr void ItOStreamer_First(ostream& stream, const _InIt& it)
{
    stream << (it->first);
}

template<input_iterator _InIt>
constexpr void ItOStreamer_Second(ostream& stream, const _InIt& it)
{
    stream << (it->second);
}

template<input_iterator _InIt>
void List(ostream& stream, _InIt begin, _InIt end)
{
    StreamList::List<_InIt, ItOStreamer_Deref<_InIt>>(stream, begin, end);
}

template<class _It>
concept iterator_to_pair = input_iterator<_It> && requires(_It it)
{
    it->first;
    it->second;
};

template<iterator_to_pair _InIt>
void List(ostream& stream, _InIt begin, _InIt end)
{
    StreamList::List<_InIt, ItOStreamer_First<_InIt>, ItOStreamer_Second<_InIt>>(stream, begin, end);
}

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

template<class _Container>
concept iteratorable_of_pairs = iterable<_Container> && requires(_Container _Cont)
{
    { begin(_Cont) } -> iterator_to_pair;
};

template<iteratorable_of_pairs _Container>
void ListKeys(ostream& stream, const _Container& options)
{
    using _InIt = decltype(begin(options));
    StreamList::List<_InIt, ItOStreamer_First<_InIt>>(stream, begin(options), end(options));
}

// Awaits a string from the user, prefixing each attempt with "> ".
// Able to take multiple inputs "at once" if the user inserts spaces between prompts.
string Prompt();

// Awaits a non-empty (and not-exclusively-whitespace) string from the user, prefixing each attempt with "> ".
string PromptLine();

// Prompts the user for a non-empty (and not-exclusively-whitespace) string, prefixing each attempt with "> ".
string Prompt(const string& prompt);

// Prompts the user for a non-empty (and not-exclusively-whitespace) string, prefixing each attempt with "> ".
string PromptLine(const string& prompt);

template<class _Container>
concept iterable_of_string_comparable = iterable<_Container> && requires(_Container _Cont, string str)
{
    { str == (*begin(_Cont)) } -> same_as<bool>;
};

// Has the user already provided a response to cin that we can use?
bool IsInputExisting();

// Prompts the user for a valid option from the provided list.
template<iterable_of_string_comparable _Container = initializer_list<const char*>>
auto PromptOption(const string& prompt, const _Container& options)
{
    while (IsInputExisting())
    {
        string input;
        cin >> input;

        for (const auto& opt : options)
        {
            if (input == opt)
            {
                return opt;
            }
        }
    }

    cout << prompt << '\n';
    List(cout, options);
    while (true)
    {
        string input = Prompt();

        for (const auto& opt : options)
        {
            if (input == opt)
            {
                return opt;
            }
        }
    }
}

template<class _Container1, class _Container2>
concept interchangeable_value_type = iterable<_Container1> && iterable<_Container2> && requires(_Container1 _Cont1, _Container2 _Cont2, bool testBool)
{
    (testBool ? (*begin(_Cont1)) : (*begin(_Cont2)));
};

// Prompts the user for a valid option from the provided list.
// Hidden options allow for options that are valid but aren't listed as part of the prompt (saving clutter).
template<iterable_of_string_comparable _VisibleContainer = initializer_list<const char*>, iterable_of_string_comparable _HiddenContainer = initializer_list<const char*>>
auto PromptOptionWithHidden(const string& prompt, const _VisibleContainer& options, const _HiddenContainer& hiddenOptions)
    requires(interchangeable_value_type<_VisibleContainer, _HiddenContainer>)
{
    while (IsInputExisting())
    {
        string input;
        cin >> input;

        for (const auto& opt : options)
        {
            if (input == opt)
            {
                return opt;
            }
        }

        for (const auto& opt : hiddenOptions)
        {
            if (input == opt)
            {
                return opt;
            }
        }
    }

    cout << prompt << '\n';
    List(cout, options);
    while (true)
    {
        string input = Prompt();

        for (const auto& opt : options)
        {
            if (input == opt)
            {
                return opt;
            }
        }

        for (const auto& opt : hiddenOptions)
        {
            if (input == opt)
            {
                return opt;
            }
        }
    }
}

template<class _Container>
concept iterable_of_string_comparable_keys = iterable<_Container> && requires(_Container _Cont, string str)
{
    { str == (begin(_Cont)->first) } -> same_as<bool>;
};

// Prompts the user for a valid option from the provided list.
// Hidden options allow for options that are valid but aren't listed as part of the prompt (saving clutter).
template<iterable_of_string_comparable_keys _Container>
auto PromptKey(const string& prompt, const _Container& options)
{
    while (IsInputExisting())
    {
        string input;
        cin >> input;

        for (const auto& [key, _] : options)
        {
            if (input == key)
            {
                return key;
            }
        }
    }

    cout << prompt << '\n';
    ListKeys(cout, options);
    while (true)
    {
        string input = Prompt();

        for (const auto& [key, _] : options)
        {
            if (input == key)
            {
                return key;
            }
        }
    }
}
