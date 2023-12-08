#ifndef Wizard_hpp
#define Wizard_hpp

#include "utilities/pch/utilities.hpp"
#include "entities/Entity.hpp"
#include "entities/NPC.hpp"

class Wizard final :
    public NPC
{
public:
    void DoInteraction_Grab ( ) override;
    void DoInteraction_Bread( ) override;
    void DoInteraction_Sword( ) override;
    void DoInteraction_Gold ( ) override;

    void DoInteraction_Talk(Topic topic) override;
    void DoInteraction_Potion(Potion potion) override;

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
