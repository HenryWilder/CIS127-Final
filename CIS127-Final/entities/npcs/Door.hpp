#ifndef Door_hpp
#define Door_hpp

#include "utilities/pch/utilities.hpp"
#include "entities/Entity.hpp"
#include "entities/NPC.hpp"

class Door final :
    public Entity
{
public:
    void DoInteraction_Grab ( ) override;
    void DoInteraction_Bread( ) override;
    void DoInteraction_Sword( ) override;
    void DoInteraction_Gold ( ) override;

    void DoInteraction_Talk(Topic topic) override;
    void DoInteraction_Potion(Potion potion) override;

    constexpr EntityType GetType( ) const override
    {
        return EntityType::Door;
    }
};

#endif /* Door_hpp */
