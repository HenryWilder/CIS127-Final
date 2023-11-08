#ifndef Interactable_hpp
#define Interactable_hpp

#include "utilities.hpp"
#include "Enums.hpp"

// Entity

enum class EntityType
{
    Player,
    Door,
    Baker,
    Blacksmith,
    Wizard,
    Monster,
};

constexpr StrEnumCollection entityTypes
{
    StrEnum{ EntityType::Player,     "self",    "yourself"    },
    StrEnum{ EntityType::Door,       "door",    "the door"    },
    StrEnum{ EntityType::Baker,      "baker",   "the baker"   },
    StrEnum{ EntityType::Blacksmith, "smith",   "the smith"   },
    StrEnum{ EntityType::Wizard,     "wizard",  "the wizard"  },
    StrEnum{ EntityType::Monster,    "monster", "the monster" },
};

STR_ENUM_OPERATORS(EntityType, entityTypes);

// Abstract base for a non-player thing in the world
// All things have names, alliances, and reactions
class Entity
{
protected:
    virtual void DoInteraction_Talk_Generic         () = 0;
    virtual void DoInteraction_Talk_WineFish        ();
    virtual void DoInteraction_Talk_SkeleStock      ();
    virtual void DoInteraction_Talk_WoodpeckerSiege ();
    virtual void DoInteraction_Talk_NecroFarm       ();
    virtual void DoInteraction_Talk_Bloodmoon       ();
    virtual void DoInteraction_Talk_MoleMountain    ();
    virtual void DoInteraction_Talk_MindflayerRecipe();
    virtual void DoInteraction_Talk_GodFistfight    ();
    virtual void DoInteraction_Talk_Theseus         ();
    virtual void DoInteraction_Talk_SmithTeleken    ();
    virtual void DoInteraction_Talk_NoGarlic        ();
    virtual void DoInteraction_Talk_PenguinBattle   ();
    virtual void DoInteraction_Talk_PetMentalHealth ();
    virtual void DoInteraction_Talk_Woodchuck       ();

    virtual void DoInteraction_Potion_Predict() = 0;
    virtual void DoInteraction_Potion_Heal   () = 0;
    virtual void DoInteraction_Potion_Water  () = 0;
    virtual void DoInteraction_Potion_Wish   () = 0;
    virtual void DoInteraction_Potion_Ducks  () = 0;
    virtual void DoInteraction_Potion_Force  () = 0;
    virtual void DoInteraction_Potion_Salt   () = 0;
    virtual void DoInteraction_Potion_Ants   () = 0;
    virtual void DoInteraction_Potion_Demon  () = 0;
    virtual void DoInteraction_Potion_Fire   () = 0;
    virtual void DoInteraction_Potion_Explode() = 0;
    virtual void DoInteraction_Potion_Tree   () = 0;

public:
    virtual void DoInteraction_Grab () = 0;
    virtual void DoInteraction_Bread() = 0;
    virtual void DoInteraction_Sword() = 0;
    virtual void DoInteraction_Gold () = 0;
    
    void DoInteraction_Talk  (Topic topic);
    void DoInteraction_Potion(Potion effect);

    void RemoveFromSurroundings();

    constexpr virtual EntityType GetType() const = 0;
};

Entity* NewInteractableOfType(EntityType type);

// NPC

enum class NPCType
{
    Baker      = (int)EntityType::Baker,
    Blacksmith = (int)EntityType::Blacksmith,
    Wizard     = (int)EntityType::Wizard,
    Monster    = (int)EntityType::Monster,
};

constexpr bool IsNPCType(EntityType type)
{
    return type == EntityType::Baker
        || type == EntityType::Blacksmith
        || type == EntityType::Wizard
        || type == EntityType::Monster;
}

constexpr StrEnumCollection npcTypes
{
    StrEnum{ NPCType::Baker,      "baker"   },
    StrEnum{ NPCType::Blacksmith, "smith"   },
    StrEnum{ NPCType::Wizard,     "wizard"  },
    StrEnum{ NPCType::Monster,    "monster" },
};

STR_ENUM_OPERATORS(NPCType, npcTypes);

class NPC :
    public Entity
{
public:
    NPC(Collective collective) :
        collective(collective) {}

    constexpr NPCType GetNPCType() const
    {
        return (NPCType)GetType();
    }

    Collective GetCollective() const
    {
        return collective;
    }

private:
    Collective collective;
};

NPC* NewNPCOfType(NPCType type, Collective collective);

#endif /* Interactable_hpp */
