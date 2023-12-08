#pragma once
#include "utilities/pch/utilities.hpp"
#include "entities/Entity.hpp"
#include "entities/NPC.hpp"
#include "player-comps/player-components.hpp"

class Player final :
    public Entity,
    public Inventory,
    public Influences,
    public Health,
    public StatusEffects,
    public Luck
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
        return EntityType::Player;
    }

    void SetName(const string& value)
    {
        name = value;
    }

    const string &GetName( ) const
    {
        return name;
    }

    void Init( ) override;
    void Save(ostream &ofs) const override;
    void Load(istream &ifs) override;

private:
    string name = "";
};

extern Player player;
