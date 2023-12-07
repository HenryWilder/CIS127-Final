#ifndef Door_hpp
#define Door_hpp

#include "utilities.hpp"
#include "Interactable.hpp"

class Door final :
    public Entity
{
protected:
    void DoInteraction_Talk_Generic( ) override;

public:
    void DoInteraction_Potion(Potion potion) override;
    void DoInteraction_Grab ( ) override;
    void DoInteraction_Bread( ) override;
    void DoInteraction_Sword( ) override;
    void DoInteraction_Gold ( ) override;

    constexpr EntityType GetType( ) const override
    {
        return EntityType::Door;
    }
};

#endif /* Door_hpp */
