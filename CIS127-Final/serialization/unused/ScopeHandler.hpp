#pragma once
#if 0
#include "SerializationUtils.hpp"

class ScopeHandler
{
public:
    ScopeHandler( ) :
        scopeStack{}
    { }

    ~ScopeHandler( );

    // Currently unscoped.
    bool IsInGlobal( ) const;

    // Nesting depth of scope stack. Used for indent.
    size_t CurrentDepth( ) const;

    // Type of current scope.
    // Not valid on global scope.
    ScopeType CurrentScope( ) const;

    // Pushes a new scope of the type.
    void PushScope(ScopeType type);

    // Pops the current scope.
    void PopScope( );

private:
    stack<ScopeType> scopeStack;
};

#endif // 0
