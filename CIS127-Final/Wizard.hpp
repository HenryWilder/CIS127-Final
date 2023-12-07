#ifndef Wizard_hpp
#define Wizard_hpp

#include "utilities.hpp"
#include "Interactable.hpp"

class Wizard final :
    public NPC
{
protected:
    void DoInteraction_Talk_Generic( ) override;

public:
    void DoInteraction_Potion(Potion potion) override;
    void DoInteraction_Grab ( ) override;
    void DoInteraction_Bread( ) override;
    void DoInteraction_Sword( ) override;
    void DoInteraction_Gold ( ) override;

    Wizard( ) :
        NPC(collectives.RandomEnum( ))
    { }

    Wizard(Collective explicitCollective) :
        NPC(explicitCollective)
    { }

    constexpr EntityType GetType( ) const override
    {
        return EntityType::Wizard;
    }
};

#endif /* Wizard_hpp */
