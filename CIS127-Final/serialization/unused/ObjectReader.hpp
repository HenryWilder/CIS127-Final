#pragma once
#if 0
#include "ScopeReader.hpp"

class ObjectReaderClient :
    protected ScopeReader
{
private:
    ObjectReaderClient( ) = delete;

    unique_ptr<ListReader> _BeginListProperty(const string& expectedName)
    {
        return reader.BeginListProperty(expectedName);
    }
    unique_ptr<ObjectReader> _BeginObjectProperty(const string& expectedName)
    {
        return reader.BeginObjectProperty(expectedName);
    }

    template<istreamable T>
    void _ReadProperty(const string& expectedName, T& value)
    {
        return reader.ReadProperty(expectedName, value);
    }
};


class ObjectReader final :
    protected ObjectReaderClient
{
public:
    ObjectReader( ) = delete;
    ~ObjectReader( )
    {
        // todo: assert that EndObject() has been called
    }

    template<istreamable T>
    void ReadProperty(const string &propertyName, const T &value)
    {
        reader.Read(propertyName, value);
    }

    ListReader BeginListProperty(const string &propertyName);
    ObjectReader BeginObjectProperty(const string &propertyName);

    void EndObject( )
    {
        // todo
    }

private:
    void _DoSafetyChecks( ) const;
};
#endif