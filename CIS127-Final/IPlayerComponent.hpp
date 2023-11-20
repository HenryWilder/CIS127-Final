#pragma once
#include "IPlayerComponent.hpp"

// Player Component interface
class IPlayerComponent
{
public:
    virtual ~IPlayerComponent( )
    { }

    virtual void Init( ) = 0; // called on new game
    virtual void Save(ostream &ofs) const = 0;
    virtual void Load(istream &ifs) = 0;
};
