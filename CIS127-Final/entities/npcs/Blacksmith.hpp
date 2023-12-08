#ifndef Blacksmith_hpp
#define Blacksmith_hpp

#include "utilities/pch/utilities.hpp"
#include "entities/Entity.hpp"
#include "entities/NPC.hpp"

class Blacksmith final :
    public NPC
{
public:
    void DoInteraction_Grab ( ) override;
    void DoInteraction_Bread( ) override;
    void DoInteraction_Sword( ) override;
    void DoInteraction_Gold ( ) override;

    void DoInteraction_Talk(Topic topic) override;
    void DoInteraction_Potion(Potion potion) override;

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
