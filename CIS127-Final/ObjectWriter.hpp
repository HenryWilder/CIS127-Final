#pragma once
#if 0
#include "ScopeWriter.hpp"

class ObjectWriterClient :
    protected ScopeWriter
{
protected:
    ObjectWriterClient( ) = delete;

    unique_ptr<ListWriter> _BeginListProperty(const string &propertyName)
    {
        return writer.BeginListProperty(propertyName);
    }
    unique_ptr<ObjectWriter> _BeginObjectProperty(const string &propertyName)
    {
        return writer.BeginObjectProperty(propertyName);
    }

    template<ostreamable T>
    static void _WriteProperty(const string &propertyName, const T &value)
    {
        writer.WriteProperty<T>(propertyName, value);
    }
};


class ObjectWriter final :
    protected ObjectWriterClient
{
public:
    static constexpr ScopeType SCOPE_TYPE = ScopeType::Object;

    ObjectWriter( ) = delete;
    ~ObjectWriter( );

    void EndObject( );

    unique_ptr<ListWriter> BeginListProperty(const string &propertyName)
    {
        return _BeginListProperty(propertyName);
    }
    unique_ptr<ObjectWriter> BeginObjectProperty(const string &propertyName)
    {
        return _BeginObjectProperty(propertyName);
    }

    template<istreamable T>
    void WriteProperty(const string &propertyName, const T &value)
    {
        _DoSafetyChecks( );
        _WriteProperty<T>(propertyName, value);
    }

private:
    void _DoSafetyChecks( ) const;
};

#endif // 0
