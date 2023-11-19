#include "utilities.hpp"
#include "ListReader.hpp"

ListReader::~ListReader( )
{
    dynamic_assert(!_IsValid( ),
        "list reader is being destroyed while still valid\n"
        "make sure to call EndList() before the list destructs\n");
}

bool ListReader::IsEndOfList( ) const
{
    size_t currentIndent = _PeekIndent( );

    dynamic_assert(currentIndent <= depth,
        "trying to read child's indent\n"
        "make sure child has completed and popped before testing parent");

    return currentIndent < depth;
}

void ListReader::EndList( )
{
    dynamic_assert(_IsValid( ),
        "trying to end an invalid list\n"
        "make sure you are calling EndList() on the correct ListReader\n"
        "remember that ListReaders should not be reused");

    dynamic_assert(IsEndOfList( ),
        "trying to end an unfinished list\n"
        "make sure list has ended before popping it\n"
        "also make sure the list isn't reading a subsequent list's elements");

    _Invalidate( );
    _PopScope( );
}

void ListReader::_DoSafetyChecks( ) const
{
    // todo
}
