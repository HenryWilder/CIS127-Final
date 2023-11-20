#pragma once
#if 0
#include "utilities.hpp"

class ScopeReaderWriter
{
public:
    ~ScopeReaderWriter( )
    {

    }

protected:
    ScopeReaderWriter(size_t depth) :
        depth(depth), isValid(true)
    { }

    // Accessors

    bool _IsValid( ) const
    {
        return isValid;
    }

    // Mutators

    void _Invalidate( )
    {
        isValid = false;
    }

    const size_t depth;

private:
    bool isValid;
};

#endif // 0
