#ifndef Monster_hpp
#define Monster_hpp

#include "utilities.hpp"
#include "Interactable.hpp"
#include "Enums.hpp"

class Monster final :
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
