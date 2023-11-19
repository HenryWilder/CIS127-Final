#include "utilities.hpp"
#include "Writer.hpp"

// Writer

void Writer::PopScope()
{
    scope.PopScope();
}

unique_ptr<ListWriter> Writer::BeginListElement()
{
    _Indent();
    stream << ":\n"; // todo: figure out a good syntax
    scope.PushScope(ScopeType::List);
    return make_unique<ListWriter>(*this, CurrentDepth());
}

unique_ptr<ObjectWriter> Writer::BeginObjectElement()
{
    _Indent();
    stream << ":\n"; // todo: figure out a good syntax
    scope.PushScope(ScopeType::Object);
    return make_unique<ObjectWriter>(*this, CurrentDepth());
}

unique_ptr<ListWriter> Writer::BeginListProperty(const string& name)
{
    assert_serialization_naming_standard(name);
    _Indent();
    stream << name << ":\n";
    scope.PushScope(ScopeType::List);
    return make_unique<ListWriter>(*this, CurrentDepth());
}

unique_ptr<ObjectWriter> Writer::BeginObjectProperty(const string& name)
{
    assert_serialization_naming_standard(name);
    _Indent();
    stream << name << ":\n";
    scope.PushScope(ScopeType::Object);
    return make_unique<ObjectWriter>(*this, CurrentDepth());
}

unique_ptr<ListWriter> Writer::BeginList(const string& name)
{
    assert_serialization_naming_standard(name);
    stream << name << ":\n";
    scope.PushScope(ScopeType::List);
    return make_unique<ListWriter>(*this, CurrentDepth());
}

unique_ptr<ObjectWriter> Writer::BeginObject(const string& name)
{
    assert_serialization_naming_standard(name);
    stream << name << ":\n";
    scope.PushScope(ScopeType::Object);
    return make_unique<ObjectWriter>(*this, CurrentDepth());
}

void Writer::_Indent()
{
    ::Indent(stream, CurrentDepth());
}
