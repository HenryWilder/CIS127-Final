#ifndef Wizard_hpp
#define Wizard_hpp

#include "utilities.hpp"
#include "Interactable.hpp"
#include "Collective.hpp"

class Wizard :
    public NPC
{
protected:
    void DoInteraction_Grab () override;
    void DoInteraction_Bread() override;
    void DoInteraction_Sword() override;
    void DoInteraction_Gold () override;
    
    void DoInteraction_Talk_Generic() override;
    
    void DoInteraction_Potion_Predict() override;
    void DoInteraction_Potion_Heal   () override;
    void DoInteraction_Potion_Water  () override;
    void DoInteraction_Potion_Wish   () override;
    void DoInteraction_Potion_Ducks  () override;
    void DoInteraction_Potion_Force  () override;
    void DoInteraction_Potion_Salt   () override;
    void DoInteraction_Potion_Ants   () override;
    void DoInteraction_Potion_Demon  () override;
    void DoInteraction_Potion_Fire   () override;
    void DoInteraction_Potion_Explode() override;
    void DoInteraction_Potion_Tree   () override;
    
public:
    Wizard() :
        NPC(Collective::Random()) {}

    Wizard(const Collective& explicitCollective) :
        NPC(explicitCollective) {}

    constexpr const char* GetShortName() const override
    {
        return "wizard";
    }
};

#endif /* Wizard_hpp */
