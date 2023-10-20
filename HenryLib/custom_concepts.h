#pragma once
#include <concepts>
#include <vector>
#include <unordered_map>
using std::floating_point;
using std::integral;
using std::signed_integral;
using std::unsigned_integral;
using std::same_as;

template<typename _Ty>
concept numeric = floating_point<_Ty> || integral<_Ty>;

template<typename _Ty>
concept enumeration = std::is_enum_v<_Ty>;

template<typename _Ty, typename... _Types>
concept any_of = std::disjunction_v<std::is_same<_Ty, _Types>...>;

template<typename _Specialization, template<typename...> class _Template>
constexpr bool is_specialization_v = false;

template<template<typename...> class _Template, typename... _Args>
constexpr bool is_specialization_v<_Template<_Args...>, _Template> = true;

template<typename _Specialization, template<typename...> class _Template>
concept specialization_of = is_specialization_v<_Specialization, _Template>;

template<typename _Container>
concept container = (
    requires { typename _Container::key_type; } ||
    requires { typename _Container::difference_type; }
) &&
    requires(_Container container)
{
    typename _Container::value_type;
    container.begin();
    container.end();
    ++container.begin();
    container.begin() != container.end();
};

template<typename _Container>
concept random_access_container = requires(_Container container, size_t index)
{
    container[index];
};

template<typename _Container, typename _Ty>
concept container_of = container<_Container> &&
    same_as<typename _Container::value_type, _Ty>;

template<typename _Container, typename _Key, typename _Val>
concept keyed_container_of = container<_Container> &&
    requires { typename _Container::key_type; } &&
    same_as<typename _Container::key_type, _Key> &&
    same_as<typename _Container::value_type, _Val>;

static_assert(container<vector<int>>);
static_assert(!container<int>);
static_assert(container_of<vector<int>, int>);
static_assert(!container_of<vector<int>, float>);
static_assert(keyed_container_of<std::unordered_map<int, int>, int, int>);

template<typename _Ty>
using forward_t = std::conditional_t<(sizeof(_Ty) > sizeof(_Ty*)), const _Ty&, const _Ty>;

using flt32_t = std::conditional_t<(sizeof(float) >= (32 / 8)), float, double>;
using flt64_t = std::conditional_t<(sizeof(double) >= (64 / 8)), double, long double>;
