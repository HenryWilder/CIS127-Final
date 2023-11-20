#pragma once
#if 0
#include "ScopeReader.hpp"

class ListReaderClient :
    protected ScopeReader
{
protected:
    ListReaderClient( ) = delete;

    unique_ptr<ListReader> _BeginListElement()
    {
        return reader.BeginListElement();
    }
    unique_ptr<ObjectReader> _BeginObjectElement()
    {
        return reader.BeginObjectElement();
    }

    template<istreamable T>
    void _ReadElement(T& value)
    {
        reader.ReadElement<T>(value);
    }
};

class ListReader final :
    protected ListReaderClient
{
public:
    ListReader( ) = delete;
    ~ListReader( );

    template<ostreamable T>
    void ReadElement(const T &value)
    {
        reader.Read(value);
    }

    ListReader BeginListElement( );
    ObjectReader BeginObjectElement( );

    bool IsEndOfList( ) const;

    void EndList( );

private:
    void _DoSafetyChecks( ) const;
};
#endif