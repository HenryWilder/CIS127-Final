#pragma once
#include "IPlayerComponent.hpp"

/**
 * @brief Represents the health component of a player entity.
 */
class Health :
    IPlayerComponent
{
public:
    /**
     * @brief Get the current health of the player.
     * @return The current health of the player.
     */
    [[nodiscard]] int GetHealth( ) const
    {
        return health;
    }

    /**
     * @brief Get the maximum health of the player.
     * @return The maximum health of the player.
     */
    [[nodiscard]] int GetMaxHealth( ) const
    {
        return maxHealth;
    }

    /**
     * @brief Check if the player is dead.
     * @return True if the player is dead, false otherwise.
     */
    [[nodiscard]] bool IsDead( ) const
    {
        return health <= 0;
    }

    /**
     * @brief Heal the player by the specified amount of points.
     * @param points The amount of points to heal the player.
     * @return The quantity of health points exceeding the max health (and not added).
     *         If all points were added successfully, returns 0.
     */
    int Heal(int points);

    /**
     * @brief Damage the player by the specified amount of points.
     * @param points The amount of points to damage the player.
     * @return True if the player survived, false if the player is dead.
     */
    bool Damage(int points);

    /**
     * @brief Initialize the health component.
     *
     * This function is called when the health component is initialized.
     * It overrides the Init function from the IPlayerComponent interface.
     */
    void Init( ) override;

    /**
     * @brief Save the health component to an output stream.
     * @param ofs The output stream to save the health component to.
     *
     * This function is called when the health component is saved.
     * It overrides the Save function from the IPlayerComponent interface.
     */
    void Save(ostream &ofs) const override;

    /**
     * @brief Load the health component from an input stream.
     * @param ifs The input stream to load the health component from.
     *
     * This function is called when the health component is loaded.
     * It overrides the Load function from the IPlayerComponent interface.
     */
    void Load(istream &ifs) override;

private:
    int health = 0;     // The current health of the player.
    int maxHealth = 0;  // The maximum health of the player.
};
