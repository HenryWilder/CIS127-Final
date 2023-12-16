#ifndef Baker_hpp
#define Baker_hpp

#include "utilities/pch/utilities.hpp"
#include "entities/Entity.hpp"
#include "entities/NPC.hpp"

class Baker final :
    public NPC
{
public:
    void DoInteraction_Grab ( ) override;
    void DoInteraction_Bread( ) override;
    void DoInteraction_Sword( ) override;
    void DoInteraction_Gold ( ) override;

    void DoInteraction_Talk(Topic topic) override;
    void DoInteraction_Potion(Potion potion) override;

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

private:
    bool isCoveredInPaste = false;
};

#endif /* Baker_hpp */
