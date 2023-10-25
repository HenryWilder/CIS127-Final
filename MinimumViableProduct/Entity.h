#pragma once
#include "Utilities.h"
#include "Character.h"

enum class EntityType
{
    _ENTITY = 0, // Null entity

    _NPC    = 2, // NPC entity flag
    NPC_BAKER      = _NPC | 0,
    NPC_BLACKSMITH = _NPC | 1,

    _NUM, // Number of entity types. Used for modulo.
};

// Base class
class Entity
{
public:
    constexpr Entity() = default;

    static EntityType TypeFromName(string typeName);

    // Used for save/load and as shortname
    virtual constexpr EntityType  GetType    () const = 0;
    virtual constexpr const char* GetTypeName() const = 0;

    // What happens when the player chooses to interact with the entity?
    virtual void DoInteraction(Player& player) = 0;

    virtual void ToStream  (ostream& stream) const = 0;
    virtual void FromStream(istream& stream)       = 0;
};


// Contains NPC-specific virtuals and helpers
class BaseNPC :
    public Entity,
    public Character
{
private:
    void DoTrade(Player& player);

public:
    constexpr BaseNPC() = default;

    virtual constexpr EntityType  GetType       () const override = 0;
    virtual constexpr const char* GetTypeName   () const override = 0;
    virtual           string      GetDisplayName() const override = 0;

    virtual void DoInteraction(Player& player) override = 0;

    inline virtual void   ToStream(ostream& stream) const override { stream << inventory << '\n'; }
    inline virtual void FromStream(istream& stream)       override { stream >> inventory;         }
};


class BakerNPC :
    public BaseNPC
{
public:
    constexpr BakerNPC() = default;

    constexpr EntityType  GetType       () const override { return EntityType::NPC_BAKER; }
    constexpr const char* GetTypeName   () const override { return "baker"; }
    inline    string      GetDisplayName() const override { return "Baker"; }

    void DoInteraction(Player& player) override;
};

class BlacksmithNPC :
    public BaseNPC
{
public:
    constexpr BlacksmithNPC() = default;

    constexpr EntityType  GetType       () const override { return EntityType::NPC_BLACKSMITH; }
    constexpr const char* GetTypeName   () const override { return "blacksmith"; }
    inline    string      GetDisplayName() const override { return "Blacksmith"; }

    void DoInteraction(Player& player) override;
};

// Allocates and returns a new entity of the named type.
// Used for loading from a file.
_Ret_maybenull_ Entity* NewEntityOfType(EntityType type);

istream& operator>>(istream& stream, Entity*& entity);
ostream& operator<<(ostream& stream, const Entity* entity);
