#include "SerializationUtils.hpp"

string ScopeTypeToString(ScopeType type)
{
    switch (type)
    {
    case ScopeType::List:   return "list";   break;
    case ScopeType::Object: return "object"; break;
    default: throw new out_of_range(to_string((int)type) + " is neither List (" + to_string((int)ScopeType::List) + ") nor Object (" + to_string((int)ScopeType::Object) + ")");
    }
}

void ScopeHandler::PopScope(ScopeType expectedType)
{
    if (scopeStack.empty()) throw new out_of_range("Error: cannot pop empty stack");
    ScopeType currentType = CurrentScope();
    if (currentType != expectedType) throw new scope_mismatch(expectedType, currentType);
    scopeStack.pop_back();
}

void Writer::BeginScope(const string& name, ScopeType type)
{
    stream << name << ":\n";
    scope.PushElement();
    scope.PushScope(type);
}
void Writer::EndScope(ScopeType type)
{
    scope.PopScope(type);
}

void Writer::BeginList(const string& name)
{
    BeginScope(name, ScopeType::List);
}
void Writer::BeginList()
{
    BeginList(to_string(scope.CurrentElements()));
}
void Writer::EndList()
{
    EndScope(ScopeType::List);
}
void Writer::BeginObject(const string& name)
{
    BeginScope(name, ScopeType::Object);
}
void Writer::BeginObject()
{
    BeginObject(to_string(scope.CurrentElements()));
}
void Writer::EndObject()
{
    EndScope(ScopeType::Object);
}

void Writer::Write(const string& name, const string& value)
{
    // todo
}
void Writer::Write(const string& name, int value)
{
    // todo
}
void Writer::Write(const string& name, float value)
{
    // todo
}

size_t Reader::BeginList(const string& expectedName)
{
    // todo
    return 0;
}
bool Reader::EndOfList(size_t id)
{
    // todo
    return false;
}

size_t Reader::BeginObject(const string& expectedName)
{
    // todo
    return 0;
}
bool Reader::EndOfObject(size_t id)
{
    // todo
    return false;
}

void Reader::Read(const string& expectedName, string& value)
{
    // todo
}
void Reader::Read(const string& expectedName, int& value)
{
    // todo
}
void Reader::Read(const string& expectedName, float& value)
{
    // todo
}
