#ifndef Baker_hpp
#define Baker_hpp

#include "utilities.hpp"
#include "Interactable.hpp"
#include "Enums.hpp"

class Baker final :
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

    Baker( ) :
        NPC(collectives.RandomEnum( ))
    { }

    Baker(Collective explicitCollective) :
        NPC(explicitCollective)
    { }

    constexpr EntityType GetType( ) const override
    {
        return EntityType::Baker;
    }
};

#endif /* Baker_hpp */
