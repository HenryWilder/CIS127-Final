#pragma once
#include "custom_concepts.h"
#include <vector>
using std::vector;
using std::find;

template<specialization_of<vector> _Container>
size_t FindIndex(const _Container& container, typename _Container::value_type search_for)
{
    return find(container.begin(), container.end(), search_for) - container.begin();
}

// Container helper
template<container _Container>
struct CtrHelper
{
    using _Ty = typename _Container::value_type;
    using _Pred = bool(forward_t<_Ty> el);
    using _PredPtr = _Pred*;

    template<_PredPtr pred>
    static constexpr _PredPtr InvertPredicate = +[](forward_t<_Ty> el) { return !pred(el); };
    static constexpr _PredPtr InvertedPredicate(_PredPtr pred)
    {
        return +[](forward_t<_Ty> el) { return !pred(el); };
    }

    // Compiletime

    template<_PredPtr pred>
    static bool Every(const _Container& container)
    {
        for (forward_t<_Ty> el : container)
        {
            if (!pred(el))
            {
                return false;
            }
        }
        return true;
    }

    template<_PredPtr pred>
    static bool None(const _Container& container)
    {
        return Every<InvertPredicate<pred>>(container);
    }

    template<_PredPtr pred>
    static bool Some(const _Container& container)
    {
        return !None<pred>(container);
    }

    // Runtime

    static bool Every(const _Container& container, _PredPtr pred)
    {
        for (forward_t<_Ty> el : container)
        {
            if (!pred(el))
            {
                return false;
            }
        }
        return true;
    }

    static bool None(const _Container& container, _PredPtr pred)
    {
        return Every(container, InvertPredicate(pred));
    }

    static bool Some(const _Container& container, _PredPtr pred)
    {
        return !None(container, pred);
    }
};

bool isnumber(const char*)
{
    for ()
    {

    }
}
