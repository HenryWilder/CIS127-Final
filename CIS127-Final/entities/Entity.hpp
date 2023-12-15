#pragma once
#include "utilities/pch/utilities.hpp"
#include "helpers/enums.hpp"

/**
 * @brief Abstract base for all non-player things in the game.
 *
 * This class defines the common interface and behavior for all entities in the game.
 * All things have names, alliances, and reactions.
 */
class Entity
{
public:
    virtual ~Entity( )
    { }

    /**
     * @brief Perform the grab interaction.
     */
    virtual void DoInteraction_Grab ( ) = 0;

    /**
     * @brief Perform the bread interaction.
     */
    virtual void DoInteraction_Bread( ) = 0;

    /**
     * @brief Perform the sword interaction.
     */
    virtual void DoInteraction_Sword( ) = 0;

    /**
     * @brief Perform the gold interaction.
     */
    virtual void DoInteraction_Gold ( ) = 0;

    /**
     * @brief Perform the talk interaction with a specific topic.
     *
     * @param topic The topic of the conversation.
     */
    virtual void DoInteraction_Talk(Topic topic) = 0;

    /**
     * @brief Perform the potion interaction with a specific effect.
     *
     * @param effect The effect of the potion.
     */
    virtual void DoInteraction_Potion(Potion effect) = 0;

    /**
     * @brief Queue the entity for removal from the game world.
     */
    void RemoveFromWorld( );

    /**
     * @brief Get the type of the entity.
     * @return The type of the entity.
     */
    constexpr virtual EntityType GetType( ) const = 0;
};

/**
 * @brief Creates a new entity of the specified type.
 *
 * @param type The type of the entity to create.
 * @return A pointer to the newly created entity.
 */
Entity *NewEntityOfType(EntityType type);
