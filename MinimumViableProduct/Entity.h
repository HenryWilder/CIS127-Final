#pragma once
#include "Utilities.h"
#include "Character.h"

enum class EntType
{
    ENT_CHEST = 0,

    _NUM,
};

enum class NPCType
{
    NPC_BAKER = 0,
    NPC_BLACKSMITH,

    _NUM,
};

// EntityType is a combination of bitwise and numeric.
// The high bits are flags, the low bits are numbers.
enum class EntityType
{
    _NIL = 0,

    _ENT = 64,
    ENT_CHEST      = _ENT | (int)EntType::ENT_CHEST,

    _NPC = 128,
    NPC_BAKER      = _NPC | (int)NPCType::NPC_BAKER,
    NPC_BLACKSMITH = _NPC | (int)NPCType::NPC_BLACKSMITH,
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
    constexpr bool IsNPC() const
    {
#pragma warning( push )
#pragma warning( disable : 26827 )
        return (int)GetType() & (int)EntityType::_NPC;
#pragma warning( pop )
    }

    // What happens when the player chooses to interact with the entity?
    virtual void DoInteraction(Player& player) = 0;

    virtual void ToStream  (ostream& stream) const = 0;
    virtual void FromStream(istream& stream)       = 0;

    inline IVec2 GetPosition() const        { return pos; }
    inline void SetPosition(IVec2 position) { pos = position; }
    inline void SetPosition(int x, int y)   { pos = IVec2(x, y); }

    inline int GetX() const { return pos.x; }
    inline int GetY() const { return pos.y; }
    inline void SetX(int x) { pos.x = x; }
    inline void SetY(int y) { pos.y = y; }

private:
    IVec2 pos = IVec2(0, 0);
};


class ChestEnt :
    public Entity
{
public:
    constexpr EntityType  GetType    () const override { return EntityType::ENT_CHEST; }
    constexpr const char* GetTypeName() const override { return "chest"; }

    void DoInteraction(Player& player) override;
};



// Contains NPC-specific virtuals and helpers
class BaseNPC :
    public Entity,
    public Character
{
protected:
    void DoTrade(Player& player);

    virtual constexpr cstring_t GetLastName() const = 0;
    virtual string GenName(int nameID) const;

public:
    constexpr BaseNPC() = default;

    virtual constexpr EntityType  GetType    () const override = 0;
    virtual constexpr const char* GetTypeName() const override = 0;

    virtual void DoInteraction(Player& player) override = 0;

    inline virtual void ToStream(ostream& stream) const override
    {
        stream << name << '\n' << inventory << '\n';
    }
    inline virtual void FromStream(istream& stream) override
    {
        getline(stream, name) >> inventory;
    }

    inline void InitName(int nameID) { name = GenName(nameID); }
};


class BakerNPC :
    public BaseNPC
{
protected:
    constexpr cstring_t GetLastName() const override { return "Baker"; }

public:
    constexpr EntityType  GetType    () const override { return EntityType::NPC_BAKER; }
    constexpr const char* GetTypeName() const override { return "baker"; }

    void DoInteraction(Player& player) override;
};

class BlacksmithNPC :
    public BaseNPC
{
protected:
    constexpr cstring_t GetLastName() const override { return "Smith"; }

public:
    constexpr EntityType  GetType    () const override { return EntityType::NPC_BLACKSMITH; }
    constexpr const char* GetTypeName() const override { return "blacksmith"; }

    void DoInteraction(Player& player) override;
};

// Allocates and returns a new entity of the named type.
// Used for loading from a file.
_Ret_maybenull_ Entity* NewEntityOfType(EntityType type);

istream& operator>>(istream& stream, Entity*& entity);
ostream& operator<<(ostream& stream, const Entity* entity);
