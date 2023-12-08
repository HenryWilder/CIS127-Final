#pragma once
#include "Entity.hpp"

class NPC :
    public Entity
{
public:
    NPC(Collective collective) :
        collective(collective)
    { }

    constexpr NPCType GetNPCType( ) const
    {
        return (NPCType)GetType( );
    }

    Collective GetCollective( ) const
    {
        return collective;
    }

private:
    Collective collective;
};

NPC *NewNPCOfType(NPCType type, Collective collective);
