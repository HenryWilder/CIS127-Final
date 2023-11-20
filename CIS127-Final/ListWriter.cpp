#include "utilities.hpp"
#if 0
#include "ListWriter.hpp"

ListWriter::~ListWriter( )
{
    dynamic_assert(!ScopeWriter::_IsValid( ),
        "list writer being destructed while still valid\n"
        "make sure you have popped the list with EndList() before destroying it");
}

void ListWriter::EndList( )
{
    _DoSafetyChecks( );

    _PopScope( );
    _Invalidate( );
}

void ListWriter::_DoSafetyChecks( ) const
{
    assert_scope_type_is_matching(_CurrentScope( ), SCOPE_TYPE);
    assert_scope_depth_is_matching(_CurrentDepth( ), depth);
}
#endif