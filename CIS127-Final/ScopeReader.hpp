#pragma once
#include "ScopeReaderWriter.hpp"
#include "Reader.hpp"

class ScopeReader :
    protected ScopeReaderWriter
{
protected:
    ScopeReader(Reader &reader, size_t depth) :
        ScopeReaderWriter(depth), reader(reader)
    {}

    // Client functions for interacting with Reader

    size_t _PeekIndent( ) const
    {
        return reader.PeekIndent( );
    }

    void _PopScope( )
    {
        reader.PopScope( );
    }

    Reader &reader;
};
