#pragma once
#include "utilities/pch/utilities.hpp"
#include "helpers/enums.hpp"

// Abstract base for a non-player thing in the world
// All things have names, alliances, and reactions
class Entity
{
public:
    virtual ~Entity( )
    { }

    virtual void DoInteraction_Grab ( ) = 0;
    virtual void DoInteraction_Bread( ) = 0;
    virtual void DoInteraction_Sword( ) = 0;
    virtual void DoInteraction_Gold ( ) = 0;

    virtual void DoInteraction_Talk(Topic topic) = 0;
    virtual void DoInteraction_Potion(Potion effect) = 0;

    void RemoveFromWorld( );

    constexpr virtual EntityType GetType( ) const = 0;
};

Entity *NewEntityOfType(EntityType type);
