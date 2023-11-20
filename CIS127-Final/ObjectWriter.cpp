#include "utilities.hpp"
#if 0
#include "ObjectWriter.hpp"

ObjectWriter::~ObjectWriter( )
{
    dynamic_assert(!_IsValid( ),
        "object writer being destructed while still valid\n"
        "make sure you have popped the object with EndObject() before destroying it");
}

void ObjectWriter::EndObject( )
{
    _DoSafetyChecks( );

    _PopScope( );
    _Invalidate( );
}

void ObjectWriter::_DoSafetyChecks( ) const
{
    assert_scope_type_is_matching(_CurrentScope( ), SCOPE_TYPE);
    assert_scope_depth_is_matching(_CurrentDepth( ), depth);
}
#endif