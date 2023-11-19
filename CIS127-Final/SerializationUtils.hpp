#pragma once
#include "utilities.hpp"

/******************
 * Similar to YAML
 * 
 * In file:
 * ----------------
 * name: value
 * name: value
 * list:
 *   item
 *   item
 *   item
 * object:
 *   property: value
 *   property: value
 * list:
 *   0:
 *     property: value
 *     property: value
 *   1:
 *     property: value
 *     property: value
 * object:
 *   property: value
 *   property: value
 *   list_property:
 *     item
 *     item
 *     item
 *   list_property:
 *     item
 *     item
 *     item
 * ----------------
 * 
 * Write code:
 * ----------------
 * Writer writer(file);
 * writer.Write("name", value);
 * writer.Write("name", value);
 * {
 *   auto listId = writer.BeginList("list");
 *   for (auto item : list)
 *     writer.WriteListItem(listId, item);
 *   writer.EndList(listId);
 * }
 * {
 *   auto objectId = writer.BeginObject("object");
 *   writer.WriteProperty(objectId, "property", );
 *   writer.EndObject(objectId);
 * }
 * ----------------
 * 
 * Read code:
 * ----------------
 * Reader reader(file);
 * reader.Read("name", value);
 * reader.Read("name", value);
 * ----------------
 * 
 ******************/

// I am absolutely LOADING this section up with assertions and exceptions
// because it's the closest this program gets to running arbitrary, interpreted code.
// Assertions confirm that *I* have written the read/write code correctly.
// Exceptions confirm that the user has not invalidated their save file.

template<class _Ty>
constexpr bool is_stringlike = false;

template<>
constexpr bool is_stringlike<string> = true;

template<>
constexpr bool is_stringlike<char*> = true;

template<>
constexpr bool is_stringlike<const char*> = true;

template<size_t _Size>
constexpr bool is_stringlike<char[_Size]> = true;

template<size_t _Size>
constexpr bool is_stringlike<const char[_Size]> = true;

enum class ScopeType { List, Object };

namespace std
{
    inline string to_string(ScopeType _Val)
    {
        switch (_Val)
        {
        case ScopeType::List:   return "list";
        case ScopeType::Object: return "object";
        default:                return "unknown";
        }
    }
}

// Spaces per indent
constexpr size_t indentSize = 2;

ostream& Indent(ostream& stream, size_t depth);

inline bool serialization_naming_standard_predicate(char ch)
{
    return !(('a' <= ch && ch <= 'z')
        || ('A' <= ch && ch <= 'Z')
        || ('0' <= ch && ch <= '9')
        || (ch == '_'));
}

#define assert_serialization_naming_standard(name)\
    do {\
        dynamic_assert(name.find_first_of(' ') == string::npos, "names should use camelCase or snake_case instead of spaces");\
        dynamic_assert(islower(name.at(0)), "names should start with a lowercase letter");\
        dynamic_assert(any_of(name.cbegin(), name.cend(), serialization_naming_standard_predicate), "names should contain only letters, numbers, and underscores");\
    } while (false)

#define assert_scope_type_is_matching(expectedScope, currentScope)\
    do {\
    dynamic_assert((expectedScope) == (currentScope),\
        "scope mismatch\n"\
        "expected " + to_string(expectedScope) + "; got " + to_string(currentScope) + "\n"\
        "make sure scope pushes and pops are of matching type\n"\
        "also make sure that pushes and pops are balanced");\
    } while (false)

#define assert_scope_depth_is_matching(expectedDepth, currentDepth)\
    do {\
        dynamic_assert((expectedDepth) == (currentDepth),\
            "depth mismatch\n"\
            "expected " + to_string(expectedDepth) + "; got " + to_string(currentDepth) + "\n"\
            "make sure scope pushes and pops are balanced");\
    } while (false)
