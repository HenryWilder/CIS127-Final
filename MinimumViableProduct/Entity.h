#pragma once
#include "Utilities.h"
#include "Character.h"

// Base class
class Entity
{
public:
    // The name of the category
    // This is what the user would type in to access the entity
    // Should be short and simple, like "chest" or "cook"
    virtual string GetCategoryName() const = 0;

    // What happens when the player chooses to interact with the entity?
    virtual void DoInteraction(Player& player) = 0;

    virtual void ToStream  (ostream& stream) const = 0;
    virtual void FromStream(istream& stream) = 0;
};

istream& operator>>(istream& stream, Entity& entity);
ostream& operator<<(ostream& stream, const Entity& entity);


class BaseNPC :
    public Entity,
    public Character
{
public:
    virtual string GetCategoryName() const override = 0;
    virtual string GetName() const override = 0;

    void DoTrade(Player& player);
    virtual void DoInteraction(Player& player) override = 0;

    inline virtual void ToStream(ostream& stream) const override
    {
        stream << inventory << '\n';
    }
    inline virtual void FromStream(istream& stream) override
    {
        stream >> inventory;
    }
};


class BakerNPC :
    public BaseNPC
{
public:
    inline string GetCategoryName() const override
    {
        return "baker";
    }
    inline string GetName() const override
    {
        return "baker";
    }

    void DoInteraction(Player& player) override;
};
