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

string ScopeTypeToString(ScopeType type);

class scope_mismatch : public runtime_error
{
public:
    // Not sure how to handle the dangling pointer... There's no default runtime_error constructor...
    scope_mismatch(ScopeType expected, ScopeType encountered) :
        runtime_error("Scope mismatch error: expected " + ScopeTypeToString(expected) + ", got " + ScopeTypeToString(encountered)) {}
};

struct ScopeID
{
public:
    friend class Reader;
    friend class Writer;

    ScopeID(const ScopeID&) = default;

    // Cannot be reassigned, only constructed.
    ScopeID& operator=(const ScopeID&) = delete;

    ~ScopeID()
    {
        dynamic_assert(IsPopped(), "scope at index " + to_string(index) + " was not closed before being popped off of the stack");
    }

private:
    static constexpr size_t npos = (size_t)(-1);

    ScopeID(_In_range_(!=, npos) size_t index) :
        index(index) {}

    // Signifies that this scope has been popped and marks that it cannot be popped again.
    void Pop()
    {
        index = npos;
    }

    bool IsValid() const
    {
        return index != npos;
    }

    bool IsPopped() const
    {
        return index == npos;
    }

    size_t index;
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

    ~ScopeHandler()
    {
        dynamic_assert(scopeStack.empty(), "Scope handler is being destroyed, but not all scopes have been closed");
    }

    // Nesting depth of scope stack. Used for indent.
    size_t CurrentDepth() const
    {
        return scopeStack.size();
    }

    // Pushes a new scope of the type.
    void PushScope(ScopeType type)
    {
        scopeStack.emplace(type);
    }

    // Pops the current scope. Throws an exception if the scope isn't the expected type.
    void PopScope(ScopeType expectedType);

    // Type of current scope.
    ScopeType CurrentScope() const
    {
        dynamic_assert(!scopeStack.empty(), "global scope has no type");
        return scopeStack.top().type;
    }

    // Number of elements at the current scope. Used for unnamed elements.
    size_t CurrentElements() const
    {
        dynamic_assert(!scopeStack.empty(), "cannot count unscoped elements");
        return scopeStack.top().elements;
    }

    // Increments number of elements at the top scope.
    // Call this before pushing a new scope so that it doesn't apply to the new scope instead of the containing scope.
    void PushElement();

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

private:
    stack<Scope> scopeStack;
};

// Spaces per indent
constexpr size_t indentSize = 2;

class Writer
{
public:
    Writer(ostream& stream) :
        stream(stream) {}

private:
    string UnnamedName() const;
    _Check_return_ ScopeID BeginScope(const string& name, ScopeType type);
    _Check_return_ ScopeID BeginScope(ScopeType type);
    void EndScope(ScopeType type, ScopeID& id);

public:
    // Creates a new list scope with the name.
    // Result must be passed to a corrosponding EndList() call.
    _Check_return_ ScopeID BeginList(const string& name);
    
    // Creates a new unnamed list scope.
    // Result must be passed to a corrosponding EndList() call.
    _Check_return_ ScopeID BeginList();
    
    // Closes the current list scope.
    void EndList(ScopeID& id);

    // Creates a new object scope.
    // Result must be passed to a corrosponding EndObject() call.
    _Check_return_ ScopeID BeginObject(const string& name);

    // Creates a new unnamed object scope.
    // Result must be passed to a corrosponding EndObject() call.
    _Check_return_ ScopeID BeginObject();
    
    // Closes the current object scope.
    void EndObject(ScopeID& id);

private:
    void Indent()
    {
        streamsize indent = (streamsize)(indentSize * scope.CurrentDepth());
        stream << setfill(' ') << setw(indent) << "";
    }

    template<ostreamable _Ty>
    void WriteValue(const _Ty& value)
    {
        if (scope.IsInScope()) scope.PushElement();
        if constexpr (is_stringlike<_Ty>) stream << '"' << value << '"';
        else                              stream        << value;
        stream << '\n';
    }

public:
    // Writes the nameless property to the stream.
    // If the type is string-like, the value is enclosed in quote marks - allowing for whitespace to be included at the start and end.
    template<ostreamable _Ty>
    void Write(const _Ty& value)
    {
        Indent();
        WriteValue(value);
    }

    // Writes the named property to the stream.
    template<ostreamable _Ty>
    void Write(const string& name, const _Ty& value)
    {
        dynamic_assert(name.find(' ') == string::npos, "please avoid putting spaces in property names to minimize confusion");
        Indent();
        stream << name << ": ";
        WriteValue(value);
    }

private:
    ScopeHandler scope;
    ostream& stream;
};

class Reader
{
public:
    Reader(istream& stream) :
        stream(stream) {}

private:
    // Marks that a scope is expected to begin, and should have the name given.
    _Check_return_ ScopeID BeginScope(ScopeType type, const string& expectedName);

    // Marks that a scope is expected to begin, and its name should be the number of elements in the scope so far.
    _Check_return_ ScopeID BeginScope(ScopeType type);

    // Returns true if the scope has closed, as marked by the indentation having changed.
    // Should be called as the condition of a while loop.
    _Check_return_ bool IsEndOfScope(ScopeType type, ScopeID& id);

public:
    // Marks that a list is expected to begin, and should have the name given.
    _Check_return_ ScopeID BeginList(const string& expectedName);

    // Marks that a list is expected to begin, and its name should be the number of elements in the scope so far.
    _Check_return_ ScopeID BeginList();

    // Returns true if the list has closed, as marked by the indentation having changed.
    // Should be called as the condition of a while loop.
    _Check_return_ bool IsEndOfList(ScopeID& id);

    // Marks that a object is expected to begin, and should have the name given.
    _Check_return_ ScopeID BeginObject(const string& expectedName);

    // Marks that a object is expected to begin, and its name should be the number of elements in the scope so far.
    _Check_return_ ScopeID BeginObject();

    // Closes object scope.
    // Throws an exception if there are still elements at the same indentation that immediately follow this line.
    void EndObject(ScopeID& id);

private:
    // Extraction operator for all types except string. Getline for string.
    template<istreamable _Ty>
    void Extract(string value, _Ty& result)
    {
        istringstream iss(value);
        _Ty resultProxy;
        iss >> resultProxy;
        if (iss.fail()) throw new runtime_error("type mismatch for property \"" + value + "\"");
        result = resultProxy;
    }

public:

    // Reads the nameless property from the stream.
    // Throws an exception if there is an extraction error.
    template<istreamable _Ty>
    void Read(_Ty& value)
    {
        string line;
        getline(stream, line);
        Extract(line, value);
    }

    // Reads the named property from the stream.
    // Throws an exception if there is a name mismatch.
    template<istreamable _Ty>
    void Read(const string& expectedName, _Ty& value)
    {
        dynamic_assert(expectedName.find(' ') == string::npos, "please avoid putting spaces in property names to minimize confusion");
        string line;
        getline(stream, line);

        size_t nameStartPos = line.find_first_not_of(' ');
        if (nameStartPos == string::npos)
            throw new runtime_error("Line is empty or whitespace");

        size_t separatorPos = line.find(':');
        if (separatorPos == string::npos)
            throw new runtime_error("Line is missing property separator (expected `name: value`; missing ':')");

        string propertyName = line.substr(nameStartPos, separatorPos - nameStartPos);
        if (propertyName != expectedName)
            throw new runtime_error("Expected property \"" + expectedName + "\", got \"" + propertyName + "\"");

        try
        {
            Extract(line.substr(separatorPos + 1), value);
        }
        catch (runtime_error* err)
        {
            throw new runtime_error("On property " + propertyName + ":\n  " + err->what());
        }
    }

private:
    ScopeHandler scope;
    istream& stream;
};

template<>
void Reader::Extract<string>(string value, string& result);
