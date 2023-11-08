#pragma once
#include "utilities.hpp"

enum class Argument
{
    Default = 0,
    Current = 1,
};

class StreamList
{
private:
    StreamList() :
        start(""),
        end("\n"),
        prefix("- "),
        suffix(""),
        separator("\n"),
        memberSeparator(": "),
        spaceMembersEvenly(false)
    {}

    static StreamList& Get()
    {
        static StreamList singleton;
        return singleton;
    }

    template<class _Ty>
    struct Member
    {
    public:
        Member(_Ty _default) :
            value({ _default }), defaultValue(_default) {}

        operator _Ty() const
        {
            return value.top();
        }

        Member& operator=(_Ty newValue)
        {
            return (value.top() = newValue), *this;
        }

        void Push(const variant<_Ty, Argument>& argument)
        {
            _Ty newValue;
            if (holds_alternative<_Ty>(argument))
            {
                newValue = get<_Ty>(argument);
            }
            else
            {
                switch (get<Argument>(argument))
                {
                case Argument::Default: newValue = Default();    break;
                case Argument::Current: newValue = (_Ty)(*this); break;
                }
            }
            value.push(newValue);
        }

        void Pop()
        {
            value.pop();
        }

        _Ty Default() const
        {
            return defaultValue;
        }

    private:
        stack<_Ty> value;
        _Ty const defaultValue;
    };

public:
    static void Push(
        const variant<const char*, Argument>& start,
        const variant<const char*, Argument>& end,
        const variant<const char*, Argument>& prefix,
        const variant<const char*, Argument>& suffix,
        const variant<const char*, Argument>& separator,
        const variant<const char*, Argument>& memberSeparator,
        const variant<bool,        Argument>& spaceMembersEvenly)
    {
        StreamList& sl = Get();
        sl.start             .Push(start);
        sl.end               .Push(end);
        sl.prefix            .Push(prefix);
        sl.suffix            .Push(suffix);
        sl.separator         .Push(separator);
        sl.memberSeparator   .Push(memberSeparator);
        sl.spaceMembersEvenly.Push(spaceMembersEvenly);
    }

    static void Pop()
    {
        StreamList& sl = Get();
        sl.start             .Pop();
        sl.end               .Pop();
        sl.prefix            .Pop();
        sl.suffix            .Pop();
        sl.separator         .Pop();
        sl.memberSeparator   .Pop();
        sl.spaceMembersEvenly.Pop();
    }

    static Member<const char*>& Start()
    {
        return Get().start;
    }
    static Member<const char*>& End()
    {
        return Get().end;
    }
    static Member<const char*>& Prefix()
    {
        return Get().prefix;
    }
    static Member<const char*>& Suffix()
    {
        return Get().suffix;
    }
    static Member<const char*>& Separator()
    {
        return Get().separator;
    }
    static Member<const char*>& MemberSeparator()
    {
        return Get().memberSeparator;
    }
    static Member<bool>& SpaceMembersEvenly()
    {
        return Get().spaceMembersEvenly;
    }

private:
    Member<const char*> start;
    Member<const char*> end;
    Member<const char*> prefix;
    Member<const char*> suffix;
    Member<const char*> separator;
    Member<const char*> memberSeparator;
    Member<bool> spaceMembersEvenly;
};

template<ostreamable _Ty>
size_t MeasureStreamable(_Ty item)
{
    ostringstream streamSimulator;
    streamSimulator << item;
    return streamSimulator.width();
}

template<ostreamable _Ty>
void _ListItem(ostream& stream, size_t width, const _Ty& item)
{
    stream << StreamList::Prefix() << setw(width) << item << StreamList::Suffix();
}

template<ostreamable _Ty1, ostreamable _Ty2>
void _ListItem(ostream& stream, size_t width1, size_t width2, const _Ty1& item1, const _Ty2& item2)
{
    stream << StreamList::Prefix() << setw(width1) << item1 << StreamList::MemberSeparator() << setw(width2) << item2 << StreamList::Suffix();
}

template<input_iterator _InIt>
void List(ostream& stream, _InIt begin, _InIt end)
{
    size_t width = 0;
    if (StreamList::SpaceMembersEvenly())
    {
        for (_InIt it = begin; it != end; ++it)
        {
            width = max(width, MeasureStreamable(*it));
        }
    }

    stream << StreamList::Start();
    _ListItem(stream, width, *begin);
    for (++begin; begin != end; ++begin)
    {
        _ListItem(stream << StreamList::Separator(), width, *begin);
    }
    stream << StreamList::End();
}

template<input_iterator _InIt>
void List(ostream& stream, _InIt begin, _InIt end)
    requires(specialization_of<remove_const_t<remove_reference_t<decltype(*begin)>>, pair>)
{
    size_t width1{ 0 }, width2{ 0 };
    if (StreamList::SpaceMembersEvenly())
    {
        for (_InIt it = begin; it != end; ++it)
        {
            width1 = max<size_t>(width1, MeasureStreamable(it->first));
            width2 = max<size_t>(width2, MeasureStreamable(it->second));
        }
    }

    stream << StreamList::Start();
    _ListItem(stream, width1, width2, begin->first, begin->second);
    for (++begin; begin != end; ++begin)
    {
        _ListItem(stream << StreamList::Separator(), width1, width2, begin->first, begin->second);
    }
    stream << StreamList::End();
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

template<iterable _Container>
void ListKeys(ostream& stream, const _Container& options)
    requires(specialization_of<remove_const_t<remove_reference_t<decltype(*begin(options))>>, pair>)
{
    size_t width = 0;
    if (StreamList::SpaceMembersEvenly())
    {
        for (auto it = begin(options); it != end(options); ++it)
        {
            width = max(width, MeasureStreamable(it->first));
        }
    }

    auto it = begin(options);

    stream << StreamList::Start();
    _ListItem(stream, width, it->first);
    for (++it; it != end(options); ++it)
    {
        _ListItem(stream << StreamList::Separator(), width, it->first);
    }
    stream << StreamList::End();
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
