#include "utilities.hpp"
#if 0
#include "ScopeHandler.hpp"

ScopeHandler::~ScopeHandler()
{
    dynamic_assert(IsInGlobal(),
        "scope handler is being destroyed, but not all scopes have been closed.\n"
        "there are still " + to_string(CurrentDepth()) + " scopes remaining.\n"
        "current scope: " + to_string(CurrentScope()));
}

// Currently unscoped.
bool ScopeHandler::IsInGlobal() const
{
    return scopeStack.empty();
}

// Nesting depth of scope stack. Used for indent.
size_t ScopeHandler::CurrentDepth() const
{
    return scopeStack.size();
}

void ScopeHandler::PushScope(ScopeType type)
{
    scopeStack.emplace(type);
}

void ScopeHandler::PopScope()
{
    dynamic_assert(!IsInGlobal(), "cannot pop global scope");
    scopeStack.pop();
}

ScopeType ScopeHandler::CurrentScope() const
{
    dynamic_assert(!IsInGlobal(), "global scope has no type.\nremember to test !IsInGlobal() first.");
    return scopeStack.top();
}

#endif