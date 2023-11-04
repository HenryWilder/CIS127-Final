#ifndef Interactable_hpp
#define Interactable_hpp

#include "utilities.hpp"

class Surroundings;
class Collective;

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
    
public:
    virtual ~Interactable() {}
    
    // Returns the shortname for the derived class
    virtual constexpr const char* GetShortName() const = 0;
    
    // Performs the derived class's reaction to the provided interaction type
    void DoInteraction(const string& action, const string& topicOrEffect);
};

Interactable* NewInteractableOfType(const string& shortName);

class NPC :
    public Interactable
{
public:
    NPC(const Collective& collective) :
        collective(collective) {}

    const Collective& GetCollective() const
    {
        return collective;
    }

private:
    const Collective& collective;
};

Interactable* NewNPCOfType(const string& shortName, const Collective& collective);

#endif /* Interactable_hpp */
