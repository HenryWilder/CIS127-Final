#ifndef Monster_hpp
#define Monster_hpp

#include "utilities/pch/utilities.hpp"
#include "entities/Entity.hpp"
#include "entities/NPC.hpp"

class Monster final :
    public NPC
{
public:
    void DoInteraction_Grab ( ) override;
    void DoInteraction_Bread( ) override;
    void DoInteraction_Sword( ) override;
    void DoInteraction_Gold ( ) override;

    void DoInteraction_Talk(Topic topic) override;
    void DoInteraction_Potion(Potion potion) override;

    Monster( ) :
        NPC(Collective::Monsters)
    { }

    constexpr EntityType GetType( ) const override
    {
        return EntityType::Monster;
    }

    bool CheckForesight( )
    {
        bool hadForesight = hasForesight;
        hasForesight = false;
        return hadForesight;
    }

    void GiveForesight( )
    {
        hasForesight = true;
    }

private:
    bool hasForesight = false;
};

#endif /* Monster_hpp */
