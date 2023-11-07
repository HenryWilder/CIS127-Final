#pragma once
#include "utilities.hpp"

constexpr char listItemBullet = '-';

namespace stream_list
{
    struct _Stream_List_Setter_Base {};

    template<class _Ty>
    struct _Stream_List_Setter :
        _Stream_List_Setter_Base
    {
        using type = _Ty;
    };

    struct _Stream_List_Item_Prefix :
        _Stream_List_Setter<const char*>
    {
        static type value;
        type setValue = "- ";
    };

    struct _Stream_List_Item_Separator :
        _Stream_List_Setter<const char*>
    {
        static type value;
        type setValue = "\n";
    };

    struct _Stream_List_Pair_Separator :
        _Stream_List_Setter<const char*>
    {
        static type value;
        type setValue = ": ";
    };

    template<class _Setter>
    concept StreamListSetter = derived_from<_Setter, _Stream_List_Setter_Base> && requires(_Setter setter)
    {
        typename _Setter::type;
        { _Setter::value } -> same_as<typename _Setter::type>;
        { setter.setValue } -> same_as<typename _Setter::type>;
        _Setter::value = setter.setValue;
    };

    template<StreamListSetter _Setter>
    ostream& operator<<(ostream& stream, const _Setter& setter)
    {
        return (_Setter::value = setter.setValue), stream;
    }

    struct _Stream_Lister_Element_Base {};

    template<Coutable _Ty>
    struct _Stream_Lister_Item :
        _Stream_Lister_Element_Base
    {
        const _Ty& item;

        static ostream& operator<<(ostream& stream, const _Stream_Lister_Item& lister)
        {
            return stream << _Stream_List_Item_Prefix::value << lister.item << _Stream_List_Item_Separator::value;
        }
    };

    template<Coutable _Key, Coutable _Val>
    struct _Stream_Lister_Pair :
        _Stream_Lister_Element_Base
    {
        const _Key& key;
        const _Val& val;

        static ostream& operator<<(ostream& stream, const _Stream_Lister_Pair& lister)
        {
            return stream << _Stream_List_Item_Prefix::value << lister.key << _Stream_List_Pair_Separator::value << lister.val << _Stream_List_Item_Separator::value;
        }
    };

    template<input_iterator _InIt>
    struct _Stream_Lister_Base
    {
        _InIt begin;
        _InIt end;

    protected:

        template<Coutable _TransformTo, _TransformTo(*_Transformer)(_InIt)>
        ostream& _List(ostream& stream) const
        {
            for (_InIt it = begin; it != end; ++it)
                stream << _Transformer(lister);
        }

        template<Coutable _TransformTo, _TransformTo(*_Transformer)(_InIt)>
        _List(_Transformer) -> _List<_TransformTo, _Transformer>;
    };

    template<input_iterator _InIt>
    struct _Stream_Lister :
        _Stream_Lister_Base<_InIt>
    {
        static ostream& operator<<(ostream& stream, const _Stream_Lister& lister)
        {
            return _List<_Item>(stream), stream;
        }

    private:
        static _Stream_Lister_Item _Item()
        {
            return _Stream_Lister_Item{ *it };
        }
    };

    template<input_iterator _InIt>
    struct _Stream_Associative_Lister
    {
        _InIt begin;
        _InIt end;

        static ostream& operator<<(ostream& stream, const _Stream_Associative_Lister& lister)
        {
            for (_InIt it = begin; it != end; ++it)
                stream << _Stream_Lister_Item{ *it };
        }
    };

    template<input_iterator _InIt>
    struct _Stream_Associative_Key_Lister
    {
        static ostream& operator<<(ostream& stream, const _Stream_Associative_Key_Lister& lister)
        {

        }
    };

    template<Coutable _Key, Coutable _Val>
    struct _Stream_Associative_Val_Lister
    {
        static ostream& operator<<(ostream& stream, const _Stream_Associative_Val_Lister& lister)
        {

        }
    };
}

void List();

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
template<Coutable _Ty, iterable<_Ty> _Container>
void List(ostream& stream, const _Container& options)
{
    for (const _Ty& opt : options)
        ListItem<_Ty>(stream, opt);
}

template<Coutable _Ty, iterable<_Ty> _Container>
void List(const _Container& options)
{
    List(cout, options);
}

template<Coutable _Key, Coutable _Val, associative_iterable<_Key, _Val> _Container>
void List(ostream& stream, const _Container& options, const char* separator = ": ")
{
    for (const auto& opt : options)
        ListPairItem<_Key, _Val>(stream, opt, separator);
}

template<Coutable _Key, Coutable _Val, associative_iterable<_Key, _Val> _Container>
void List(const _Container& options, const char* separator = ": ")
{
    List(cout, options, separator);
}

template<Coutable _Key, class _Val, associative_iterable<_Key, _Val> _Container>
void ListKeys(ostream& stream, const _Container& options)
{
    for (const auto& [key, _] : options)
        ListItem(stream, key);
}

template<Coutable _Key, class _Val, associative_iterable<_Key, _Val> _Container>
void ListKeys(const _Container& options)
{
    ListKeys(cout, options);
}

// Awaits a non-empty (and not-exclusively-whitespace) string from the user, prefixing each attempt with "> ".
string Prompt();

// Prompts the user for a non-empty (and not-exclusively-whitespace) string, prefixing each attempt with "> ".
string Prompt(const string& prompt);

// Prompts the user for a valid option from the provided list.
template<Coutable _Ty = string, iterable<_Ty> _VisibleContainer = initializer_list<_Ty>>
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
template<Coutable _Ty = string, iterable<_Ty> _VisibleContainer = initializer_list<_Ty>, iterable<_Ty> _HiddenContainer = initializer_list<_Ty>>
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
