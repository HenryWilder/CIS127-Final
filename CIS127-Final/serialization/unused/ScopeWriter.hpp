#pragma once
#if 0
#include "ScopeReaderWriter.hpp"
#include "Writer.hpp"

class ScopeWriter :
    protected ScopeReaderWriter
{
protected:
    ScopeWriter(Writer &writer, size_t depth) :
        ScopeReaderWriter(depth), writer(writer)
    { }

    // Writer interactions

    void _PopScope( )
    {
        writer.PopScope( );
    }

    size_t _CurrentDepth( ) const
    {
        return writer.CurrentDepth( );
    }

    ScopeType _CurrentScope( ) const
    {
        return writer.CurrentScope( );
    }

    Writer &writer;
};

#endif // 0
