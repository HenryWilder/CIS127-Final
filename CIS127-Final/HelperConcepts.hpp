#pragma once

#include "standard.hpp"

template<class _Ty>
concept Coutable = requires(_Ty x) { cout << x; };

template<class _Specialization, template<class...> class _Template>
struct is_specialization : false_type {};

template<template<class...> class _Template, class... _Args>
struct is_specialization<_Template<_Args...>, _Template> : true_type {};

template<class _Specialization, template<class...> class _Template>
constexpr bool is_specialization_v = is_specialization<_Specialization, _Template>::value;

template<class _Specialization, template<class...> class _Template>
concept specialization_of = is_specialization_v<_Specialization, _Template>;

template<class _Container, class _Ty>
concept iterable = requires(_Container _Cont)
{
    begin(_Cont);
    end(_Cont);
    { *begin(_Cont) } -> convertible_to<_Ty>;
    { ++begin(_Cont) } -> forward_iterator;
};

template<class _Container, class _Key, class _Val>
concept associative_iterable = requires(_Container _Cont)
{
    begin(_Cont);
    end(_Cont);
    { begin(_Cont)->first } -> convertible_to<_Key>;
    { begin(_Cont)->second } -> convertible_to<_Val>;
    { (++begin(_Cont))->first } -> convertible_to<_Key>;
    { (++begin(_Cont))->second } -> convertible_to<_Val>;
};

template<class _ArrayTy, class _ElemTy>
concept array_of = is_array_v<_ArrayTy> && same_as<remove_pointer<_ArrayTy>, _ElemTy>;

template<class _ArrayTy, class _ElemTy>
concept bounded_array_of = is_bounded_array_v<_ArrayTy> && same_as<remove_pointer<_ArrayTy>, _ElemTy>;
