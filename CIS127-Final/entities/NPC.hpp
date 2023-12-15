#pragma once
#include "Entity.hpp"

/**
 * @brief Represents a Non-Player Character (NPC) entity.
 */
class NPC :
    public Entity
{
public:
    /**
     * @brief Constructs an NPC object with the specified collective.
     *
     * @param collective The collective to which the NPC belongs.
     */
    NPC(Collective collective) :
        collective(collective)
    { }

    /**
     * @brief Gets the type of the NPC.
     *
     * @return The type of the NPC.
     */
    constexpr NPCType GetNPCType( ) const
    {
        return (NPCType)GetType( );
    }

    /**
     * @brief Gets the collective to which the NPC belongs.
     *
     * @return The collective of the NPC.
     */
    Collective GetCollective( ) const
    {
        return collective;
    }

private:
    /**
     * @brief The collective this NPC belongs to.
     *
     * This is used for performing "influence" checks.
     */
    Collective collective;
};

/**
 * @brief Creates a new NPC of the specified type and collective.
 *
 * @param type The type of the NPC.
 * @param collective The collective to which the NPC belongs.
 * @return A pointer to the newly created NPC.
 */
NPC *NewNPCOfType(NPCType type, Collective collective);
