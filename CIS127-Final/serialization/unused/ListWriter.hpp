#pragma once
#if 0
#include "ScopeWriter.hpp"

class ListWriterClient :
    protected ScopeWriter
{
protected:
    ListWriterClient( ) = delete;

    unique_ptr<ListWriter> _BeginListElement( )
    {
        return writer.BeginListElement( );
    }
    unique_ptr<ObjectWriter> _BeginObjectElement( )
    {
        return writer.BeginObjectElement( );
    }

    template<ostreamable T>
    void _WriteElement(const T &value)
    {
        writer.WriteElement<T>(value);
    }
};


class ListWriter final :
    protected ListWriterClient
{
public:
    static constexpr ScopeType SCOPE_TYPE = ScopeType::List;

    ListWriter( ) = delete;
    ~ListWriter( );

    void EndList( );

    unique_ptr<ListWriter> BeginListElement( )
    {
        return _BeginListElement( );
    }
    unique_ptr<ObjectWriter> BeginObjectElement( )
    {
        return _BeginObjectElement( );
    }

    template<ostreamable T>
    void WriteElement(const T &value)
    {
        _DoSafetyChecks( );
        _WriteElement<T>(value);
    }

private:
    void _DoSafetyChecks( ) const;
};

#endif // 0
