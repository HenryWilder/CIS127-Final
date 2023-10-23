#pragma once
#include <algorithm>
#include <concepts>
#include <vector>

// A container that can be iterated over
template<class _Container>
concept Iterable = requires(_Container _Cont)
{
    typename _Container::value_type;
    _Cont.begin();
    _Cont.end();
    _Cont.erase(_Cont.begin(), _Cont.end());
};
static_assert(Iterable<std::vector<int>>);

// A predicate (bool-returning function/lambda) that only requires one parameter
template<class _Pr, class _Ty>
concept UnaryPredicate = requires(_Pr _Pred, _Ty _Elem)
{
    { _Pred(_Elem) } -> std::same_as<bool>;
};
static_assert(UnaryPredicate<bool(int), int>);
static_assert(UnaryPredicate<bool(*)(int), int>);
static_assert(UnaryPredicate<decltype([](int) { return true; }), int> );

// A predicate (bool-returning function/lambda) that requires two parameters
template<class _Pr, class _Ty>
concept BinaryPredicate = requires(_Pr _Pred, _Ty _Elem)
{
    { _Pred(_Elem, _Elem) } -> std::same_as<bool>;
};
static_assert(BinaryPredicate<bool(int, int), int>);
static_assert(BinaryPredicate<bool(*)(int, int), int>);
static_assert(BinaryPredicate<decltype([](int, int) { return true; }), int> );

template<typename _Pr, typename _Container>
concept UnaryPredicateFor = UnaryPredicate<_Pr, typename _Container::value_type>;


template<Iterable _Container, UnaryPredicateFor<_Container> _Pr>
size_t count_if(const _Container& _Cont, _Pr _Pred)
{
    return std::count_if(_Cont.begin(), _Cont.end(), _Pred);
}

template<Iterable _Container, UnaryPredicateFor<_Container> _Pr>
bool all_of(const _Container& _Cont, _Pr _Pred)
{
    return std::all_of(_Cont.begin(), _Cont.end(), _Pred);
}

template<Iterable _Container, UnaryPredicateFor<_Container> _Pr>
bool any_of(const _Container& _Cont, _Pr _Pred)
{
    return std::any_of(_Cont.begin(), _Cont.end(), _Pred);
}

template<Iterable _Container, UnaryPredicateFor<_Container> _Pr>
bool none_of(const _Container& _Cont, _Pr _Pred)
{
    return std::none_of(_Cont.begin(), _Cont.end(), _Pred);
}

template<Iterable _Container, UnaryPredicateFor<_Container> _Pr>
void erase_if(_Container& _Cont, _Pr _Pred)
{
    _Cont.erase(std::remove_if(_Cont.begin(), _Cont.end(), _Pred), _Cont.end());
}
