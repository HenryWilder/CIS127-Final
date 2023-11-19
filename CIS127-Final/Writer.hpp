#pragma once
#include "SerializationUtils.hpp"
#include "ScopeHandler.hpp"

class ListWriter;
class ObjectWriter;

// Writer

class Writer
{
private:
    void _Indent();

    template<ostreamable T>
    void _Insert(const T& value)
    {
        if constexpr (is_stringlike<T>)
        {
            stream << '"' << value << '"';
        }
        else if constexpr (is_same_v<T, bool>)
        {
            stream << (value ? "true" : "false");
        }
        else
        {
            stream << value;
        }
        stream << '\n';
    }

private: // Available to both ListWriter and ObjectWriter
    friend class ScopeWriter;

    void PopScope();

    size_t CurrentDepth() const
    {
        return scope.CurrentDepth();
    }
    ScopeType CurrentScope() const
    {
        return scope.CurrentScope();
    }

private: // Available to ListWriter
    friend class ListWriterClient;

    unique_ptr<ListWriter> BeginListElement();
    unique_ptr<ObjectWriter> BeginObjectElement();

    template<ostreamable T>
    void WriteElement(const T& value)
    {
        _Indent();
        _Insert(value);
    }

private: // Available to ObjectWriter
    friend class ObjectWriterClient;

    unique_ptr<ListWriter> BeginListProperty(const string& name);
    unique_ptr<ObjectWriter> BeginObjectProperty(const string& name);

    // Writes the named object property to the stream.
    // Should not be used in global scope.
    template<ostreamable T>
    void WriteProperty(const string& name, const T& value)
    {
        assert_serialization_naming_standard(name);

        _Indent();
        stream << name << ": ";
        _Insert(value);
    }

public:
    Writer(ostream& stream) :
        stream(stream) {}

    unique_ptr<ListWriter> BeginList(const string& name);
    unique_ptr<ObjectWriter> BeginObject(const string& name);

    // Writes the named global-scope item to the stream.
    template<ostreamable T>
    void Write(const string& name, const T& value)
    {
        assert_serialization_naming_standard(name);

        stream << name << ": ";
        _Insert(value);
    }

private:
    ScopeHandler scope;
    ostream& stream;
};
