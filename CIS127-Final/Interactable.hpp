#ifndef Interactable_hpp
#define Interactable_hpp

#include "utilities.hpp"
#include "Enums.hpp"

// Interactable

enum class InteractableType
{
    Player,
    Door,
    Baker,
    Blacksmith,
    Wizard,
    Monster,
};

constexpr StrEnumCollection interactableTypes
{
    StrEnum{ InteractableType::Player,     "self"    },
    StrEnum{ InteractableType::Door,       "door"    },
    StrEnum{ InteractableType::Baker,      "baker"   },
    StrEnum{ InteractableType::Blacksmith, "smith"   },
    StrEnum{ InteractableType::Wizard,     "wizard"  },
    StrEnum{ InteractableType::Monster,    "monster" },
};

STR_ENUM_OPERATORS(InteractableType, interactableTypes);

// Abstract base for a non-player thing in the world
// All things have names, alliances, and reactions
class Interactable
{
protected:
    virtual void DoInteraction_Grab () = 0;
    virtual void DoInteraction_Bread() = 0;
    virtual void DoInteraction_Sword() = 0;
    virtual void DoInteraction_Gold () = 0;
    
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
    
    void DoInteraction_Talk  (const string& topic);
    void DoInteraction_Potion(const string& effect);

    void Destroy();
    
public:
    virtual ~Interactable() {}
    
    // Returns the shortname for the derived class
    virtual constexpr const char* GetShortName() const = 0;
    
    // Performs the derived class's reaction to the provided interaction type
    void DoInteraction(const string& action, const string& topicOrEffect);
};

Interactable* NewInteractableOfType(InteractableType type);

// NPC

enum class NPCType
{
    Baker,
    Blacksmith,
    Wizard,
    Monster,
};

constexpr StrEnumCollection npcTypes
{
    StrEnum{ NPCType::Baker,      "baker"   },
    StrEnum{ NPCType::Blacksmith, "smith"   },
    StrEnum{ NPCType::Wizard,     "wizard"  },
    StrEnum{ NPCType::Monster,    "monster" },
};

STR_ENUM_OPERATORS(NPCType, npcTypes);

class NPC :
    public Interactable
{
public:
    NPC(Collective collective) :
        collective(collective) {}

    Collective GetCollective() const
    {
        return collective;
    }

private:
    Collective collective;
};

NPC* NewNPCOfType(NPCType type, Collective collective);

#endif /* Interactable_hpp */
