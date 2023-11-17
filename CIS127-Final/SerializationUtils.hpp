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
 * WriteProperty(file, "name", value);
 * WriteProperty(file, "name", value);
 * ----------------
 * 
 * Read code:
 * ----------------
 * 
 * ----------------
 * 
 ******************/

enum class ScopeType { List, Object };

string ScopeTypeToString(ScopeType type);

class scope_mismatch : public runtime_error
{
public:
    // Not sure how to handle the dangling pointer... There's no default runtime_error constructor...
    scope_mismatch(ScopeType expected, ScopeType encountered) :
        runtime_error("Scope mismatch error: expected " + ScopeTypeToString(expected) + ", got " + ScopeTypeToString(encountered)) {}
};

class ScopeHandler
{
private:
    struct Scope
    {
        constexpr Scope(ScopeType type) :
            type(type), elements(0) {}

        ScopeType type;
        size_t elements;
    };

public:
    ScopeHandler() :
        scopeStack{} {}

    // Nesting depth of scope stack. Used for indent.
    size_t CurrentDepth() const
    {
        return scopeStack.size();
    }

    // Pushes a new scope of the type.
    void PushScope(ScopeType type)
    {
        scopeStack.emplace_back(type);
    }

    // Pops the current scope. Throws an exception if the scope isn't the expected type.
    void PopScope(ScopeType expectedType);

    // Type of current scope.
    ScopeType CurrentScope() const
    {
        return scopeStack.back().type;
    }

    // ScopeType at the scope.
    ScopeType ScopeAt(size_t index) const
    {
        return scopeStack.at(index).type;
    }

    // Number of elements at the current scope. Used for unnamed elements.
    size_t CurrentElements() const
    {
        return scopeStack.back().elements;
    }

    // Increments number of elements at the top scope.
    // Call this before pushing a new scope so that it doesn't apply to the new scope instead of the containing scope.
    void PushElement()
    {
        ++scopeStack.back().elements;
    }

    // Number of elements at the scope.
    size_t ElementsAt(size_t index) const
    {
        return scopeStack.at(index).elements;
    }

    // Currently unscoped.
    bool IsInGlobal() const
    {
        return scopeStack.empty();
    }

    // Currently scoped.
    bool IsInScope() const
    {
        return !IsInGlobal();
    }

    // Top scope (exists and) is of the type.
    bool IsInScopeType(ScopeType type) const
    {
        return IsInScope() && CurrentScope() == type;
    }

    // Pushes a new list scope.
    void PushList()
    {
        PushScope(ScopeType::List);
    }

    // Pushes a new object scope.
    void PushObject()
    {
        PushScope(ScopeType::Object);
    }

    // Pops the current scope. Throws an exception if the scope isn't a list.
    void PopList()
    {
        PopScope(ScopeType::List);
    }

    // Pops the current scope. Throws an exception if the scope isn't a object.
    void PopObject()
    {
        PopScope(ScopeType::Object);
    }

    // Top scope is a list.
    bool IsInList() const
    {
        return IsInScopeType(ScopeType::List);
    }

    // Top scope is an object.
    bool IsInObject() const
    {
        return IsInScopeType(ScopeType::Object);
    }

private:
    vector<Scope> scopeStack;
};

class Writer
{
public:
    Writer(ostream& stream) :
        stream(stream) {}

private:
    void BeginScope(const string& name, ScopeType type);
    void EndScope(ScopeType type);

public:
    // Creates a new list scope with the name.
    void BeginList(const string& name);
    
    // Creates a new unnamed list scope.
    void BeginList();
    
    // Closes the current list scope.
    void EndList();


    // Creates a new object scope.
    void BeginObject(const string& name);
    
    // Creates a new unnamed object scope.
    void BeginObject();
    
    // Closes the current object scope.
    void EndObject();

    void Write(const string& name, const string& value);
    void Write(const string& name, int           value);
    void Write(const string& name, float         value);

private:
    ScopeHandler scope;
    ostream& stream;
};

class Reader
{
public:
    Reader(istream& stream) :
        stream(stream) {}

    size_t BeginList(const string& expectedName);
    bool EndOfList(size_t id);

    size_t BeginObject(const string& expectedName);
    bool EndOfObject(size_t id);

    void Read(const string& expectedName, string& value);
    void Read(const string& expectedName, int&    value);
    void Read(const string& expectedName, float&  value);

private:
    ScopeHandler scope;
    istream& stream;
};
