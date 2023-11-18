#include "SerializationUtils.hpp"

string ScopeTypeToString(ScopeType type)
{
    dynamic_assert(type == ScopeType::List || type == ScopeType::Object, "unrecognized ScopeType");

    switch (type)
    {
    case ScopeType::List:   return "list";
    case ScopeType::Object: return "object";
    default:                return "unknown";
    }
}

void ScopeHandler::PopScope(ScopeType expectedType)
{
    dynamic_assert(!scopeStack.empty(), "cannot pop empty stack");

    ScopeType currentType = CurrentScope();

    if (currentType != expectedType)
        throw new scope_mismatch(expectedType, currentType);

    scopeStack.pop();
}

void ScopeHandler::PushElement()
{
    dynamic_assert(!scopeStack.empty(), "Cannot push element in empty stack");

    ++scopeStack.top().elements;
}

string Writer::UnnamedName() const
{
    dynamic_assert(scope.IsInScope(), "Cannot create unnamed in global scope. Push a named scope instead.");
    return to_string(scope.CurrentElements());
}

_Check_return_
ScopeID Writer::BeginScope(const string& name, ScopeType type)
{
    stream << name << ":\n";
    if (scope.IsInScope()) scope.PushElement();
    scope.PushScope(type);
    return ScopeID(scope.CurrentDepth());
}
_Check_return_
ScopeID Writer::BeginScope(ScopeType type)
{
    return BeginScope(UnnamedName(), type);
}
void Writer::EndScope(ScopeType type, ScopeID& id)
{
    dynamic_assert(id.IsValid(), "Trying to duplicatively pop the scope; make sure not to reuse the scope variable.");
    dynamic_assert(scope.CurrentDepth() == id.index, "Scope is unbalanced; check if you popped too many/few scopes.");
    dynamic_assert(scope.CurrentScope() == type, "Scope type mismatch; check your scope types and make sure that pushes/pops are balanced.");
    id.Pop();
    scope.PopScope(type);
}

_Check_return_
ScopeID Writer::BeginList(const string& name)
{
    return BeginScope(name, ScopeType::List);
}
_Check_return_
ScopeID Writer::BeginList()
{
    return BeginScope(ScopeType::List);
}
void Writer::EndList(ScopeID& id)
{
    EndScope(ScopeType::List, id);
}
_Check_return_
ScopeID Writer::BeginObject(const string& name)
{
    return BeginScope(name, ScopeType::Object);
}
_Check_return_
ScopeID Writer::BeginObject()
{
    return BeginScope(ScopeType::Object);
}
void Writer::EndObject(ScopeID& id)
{
    EndScope(ScopeType::Object, id);
}

_Check_return_
ScopeID Reader::BeginScope(ScopeType type, const string& expectedName)
{
    string name;
    getline(stream, name);
    size_t colonPos = name.find(':');

    if (colonPos == string::npos)
        throw new runtime_error(ScopeTypeToString(type) + " head is missing colon (expected `name:`; got `name`)");

    if (name.substr(0, colonPos) != expectedName)
        throw new runtime_error("expected " + ScopeTypeToString(type) + " \"" + expectedName + "\", got \"" + name + "\"");

    scope.PushScope(type);
    return scope.CurrentDepth();
}
_Check_return_
ScopeID Reader::BeginScope(ScopeType type)
{
    dynamic_assert(scope.IsInScope(), "cannot read unnamed property in global scope");
    return BeginScope(type, to_string(scope.CurrentElements()));
}
_Check_return_
bool Reader::IsEndOfScope(ScopeType type, ScopeID& id)
{
    dynamic_assert(scope.CurrentScope() == type, "scope does not match expected; make sure that scope pushes and pops are balanced");

    size_t expectedIndent = id.index;

    streampos startOfLine = stream.tellg();
    string lineStr;
    getline(stream, lineStr);
    stream.seekg(startOfLine);

    size_t leadingSpaces = lineStr.find_first_not_of(' ');
    if (leadingSpaces == string::npos) leadingSpaces = 0;
    size_t indentDepth = leadingSpaces / indentSize;

    dynamic_assert(indentDepth <= expectedIndent, "scope is testing child's indent; make sure child scope is ended/popped first");
    if (indentDepth < expectedIndent)
    {
        id.Pop();
        scope.PopScope(type);
    }

    return id.IsPopped();
}
_Check_return_
ScopeID Reader::BeginList(const string& expectedName)
{
    return BeginScope(ScopeType::List, expectedName);
}
_Check_return_
ScopeID Reader::BeginList()
{
    return BeginScope(ScopeType::List);
}
_Check_return_
bool Reader::IsEndOfList(ScopeID& id)
{
    return IsEndOfScope(ScopeType::List, id);
}

_Check_return_
ScopeID Reader::BeginObject(const string& expectedName)
{
    return BeginScope(ScopeType::Object, expectedName);
}
_Check_return_
ScopeID Reader::BeginObject()
{
    return BeginScope(ScopeType::Object);
}
void Reader::EndObject(ScopeID& id)
{
    if (!IsEndOfScope(ScopeType::Object, id))
        throw new runtime_error("expected indentation to end following initialization of all properties for object " + to_string(id.index));
}

runtime_error* GenerateExctractException(const string& value, const string& descr, const string& got, const string& remarks = "")
{
    return new runtime_error(format("String property value (\"{}\") is {} (expected `name: \"value\"`; got `name: {}`). {}", value, descr, got, remarks));
}

template<>
void Reader::Extract<string>(string value, string& result)
{
    size_t firstNonWS, lastNonWS;

    firstNonWS = value.find_first_not_of(" \t");

    if (firstNonWS == string::npos)
        throw GenerateExctractException(value, "entirely missing/whitespace", "", "Is this property meant to be scoped?");

    lastNonWS = value.find_last_not_of(" \t");

    bool isMissingOpenQuote = value.at(firstNonWS) != '\"';
    bool isMissingCloseQuote = value.at(lastNonWS) != '\"';

    if (isMissingOpenQuote && isMissingCloseQuote)
        throw GenerateExctractException(value, "not contained within quote marks", "value");

    else if (isMissingOpenQuote)
        throw GenerateExctractException(value, "not preceeded with a opening quote mark", "value\"");

    else if (isMissingCloseQuote)
        throw GenerateExctractException(value, "not followed by a closing quote mark", "\"value", "Mult-line strings are not supported.");

    else if (firstNonWS == lastNonWS)
        throw GenerateExctractException(value, "malformed", "\"", "Mult-line strings are not supported.");

    result = value.substr(firstNonWS + 1, lastNonWS - firstNonWS - 1);
}
