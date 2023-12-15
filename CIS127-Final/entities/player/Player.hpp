#pragma once
#include "utilities/pch/utilities.hpp"
#include "entities/Entity.hpp"
#include "entities/NPC.hpp"
#include "player-comps/player-components.hpp"

/**
 * @brief The Player class represents a player entity in the game.
 */
class Player final :
    public Entity,
    public Inventory,
    public Influences,
    public Health,
    public StatusEffects,
    public Luck
{
public:
    /**
     * @brief Performs the interaction of grabbing an item.
     */
    void DoInteraction_Grab ( ) override;

    /**
     * @brief Performs the interaction of using bread.
     */
    void DoInteraction_Bread( ) override;

    /**
     * @brief Performs the interaction of wielding a sword.
     */
    void DoInteraction_Sword( ) override;

    /**
     * @brief Performs the interaction of collecting gold.
     */
    void DoInteraction_Gold ( ) override;

    /**
     * @brief Performs the interaction of talking to an NPC on a specific topic.
     *
     * @param topic The topic of conversation.
     */
    void DoInteraction_Talk(Topic topic) override;

    /**
     * @brief Performs the interaction of using a potion.
     *
     * @param potion The potion to use.
     */
    void DoInteraction_Potion(Potion potion) override;

    /**
     * @brief Returns the type of the entity.
     *
     * @return The entity type, which is EntityType::Player.
     */
    constexpr EntityType GetType( ) const override
    {
        return EntityType::Player;
    }

    /**
     * @brief Sets the name of the player.
     *
     * @param value The name to set.
     */
    void SetName(const string& value)
    {
        name = value;
    }

    /**
     * @brief Returns the name of the player.
     *
     * @return The name of the player.
     */
    const string &GetName( ) const
    {
        return name;
    }

    /**
     * @brief Initializes the player.
     */
    void Init( ) override;

    /**
     * @brief Saves the player data to a file stream.
     *
     * @param ofs The output file stream to save the data to.
     */
    void Save(ostream &ofs) const override;

    /**
     * @brief Loads the player data from a file stream.
     *
     * @param ifs The input file stream to load the data from.
     */
    void Load(istream &ifs) override;

private:
    string name = "";
};

extern Player player;
