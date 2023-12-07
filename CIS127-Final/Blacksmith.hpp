#ifndef Blacksmith_hpp
#define Blacksmith_hpp

#include "utilities.hpp"
#include "Interactable.hpp"

class Blacksmith final :
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

    Blacksmith( ) :
        NPC(collectives.RandomEnum( ))
    { }

    Blacksmith(Collective explicitCollective) :
        NPC(explicitCollective)
    { }

    constexpr EntityType GetType( ) const override
    {
        return EntityType::Blacksmith;
    }
};

#endif /* Blacksmith_hpp */
