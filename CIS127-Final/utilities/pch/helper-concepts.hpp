#pragma once
#include "utilities/pch/standard.hpp"

template<class _Ty, class... _Options>
concept one_of = disjunction_v<is_same<_Ty, _Options>...>;

template<class _Ty>
concept ostreamable = requires(ostream stream, _Ty x) { stream << x; };

template<class _Ty>
concept istreamable = requires(istream stream, _Ty x) { stream >> x; };

template<class _Ty>
concept iostreamable = ostreamable<_Ty> && istreamable<_Ty>;

template<class _Specialization, template<class...> class _Template>
struct is_specialization : false_type {};

template<template<class...> class _Template, class... _Args>
struct is_specialization<_Template<_Args...>, _Template> : true_type {};

template<class _Specialization, template<class...> class _Template>
constexpr bool is_specialization_v = is_specialization<_Specialization, _Template>::value;

template<class _Specialization, template<class...> class _Template>
concept specialization_of = is_specialization_v<_Specialization, _Template>;

template<class _Container>
concept iterable = requires(_Container _Cont)
{
    begin(_Cont);
    end(_Cont);
};

template<class _Container>
concept associative_iterable = iterable<_Container> && requires(_Container _Cont)
{
    begin(_Cont)->first;
    begin(_Cont)->second;
};

template<class _ArrayTy, class _ElemTy>
concept array_of = is_array_v<_ArrayTy> && same_as<remove_pointer<_ArrayTy>, _ElemTy>;

template<class _ArrayTy, class _ElemTy>
concept bounded_array_of = is_bounded_array_v<_ArrayTy> && same_as<remove_pointer<_ArrayTy>, _ElemTy>;

template<class...>
constexpr bool is_all_same_v = false;

template<class _OnlyArg>
constexpr bool is_all_same_v<_OnlyArg> = true;

template<class _Arg1, class... _Args>
constexpr bool is_all_same_v<_Arg1, _Args...> = (... && same_as<_Arg1, _Args>);

template<class... _Args>
concept all_same = is_all_same_v<_Args...>;
