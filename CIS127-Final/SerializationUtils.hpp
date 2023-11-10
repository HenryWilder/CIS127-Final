#pragma once
#include "utilities.hpp"

template<
    specialization_of<basic_string> _String = string,
    one_of<_String, typename _String::value_type> _Separator = _String,
    specialization_of<basic_string_view> _View = basic_string_view<typename _String::value_type>,
    predicate<_View> _Func
>
constexpr void split(const _String& str, const _Separator& separator, const _Func& fn)
{
    for (_View view(str); !view.empty();)
    {
        auto sep_pos = view.find(separator);

        // Skip if would be empty, otherwise call function
        if (sep_pos)
        {
            // Call the function so it can do what it needs with the split string
            if (!fn(view.substr(0, sep_pos)))
            {
                break;
            }

            // If we got to the end, we're done
            if (sep_pos == _View::npos)
            {
                break;
            }
        }

        // Move beyond the separator
        view.remove_prefix(sep_pos + ([&]() {
            if constexpr (is_same_v<_Separator, _String>)
                return separator.size();
            else
                return 1ull;
        })());
    }
}
