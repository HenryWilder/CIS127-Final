#ifndef Player_hpp
#define Player_hpp

#include "utilities.hpp"
#include "Interactable.hpp"
#include "PlayerComponents.hpp"

class Player final :
    public Entity,
    public Inventory,
    public Influences,
    public Health,
    public StatusEffects,
    public Luck
{
protected:
    void DoInteraction_Talk_Generic        () final;
    void DoInteraction_Talk_WineFish       () final;
    void DoInteraction_Talk_SkeleStock     () final;
    void DoInteraction_Talk_WoodpeckerSiege() final;
    void DoInteraction_Talk_SmithTeleken   () final;
    void DoInteraction_Talk_PenguinBattle  () final;
    void DoInteraction_Talk_Woodchuck      () final;
    
    void DoInteraction_Potion_Predict() final;
    void DoInteraction_Potion_Heal   () final;
    void DoInteraction_Potion_Water  () final;
    void DoInteraction_Potion_Wish   () final;
    void DoInteraction_Potion_Ducks  () final;
    void DoInteraction_Potion_Force  () final;
    void DoInteraction_Potion_Salt   () final;
    void DoInteraction_Potion_Ants   () final;
    void DoInteraction_Potion_Demon  () final;
    void DoInteraction_Potion_Fire   () final;
    void DoInteraction_Potion_Explode() final;
    void DoInteraction_Potion_Tree   () final;
    
public:
    void DoInteraction_Grab () final;
    void DoInteraction_Bread() final;
    void DoInteraction_Sword() final;
    void DoInteraction_Gold () final;

    constexpr EntityType GetType() const final
    {
        return EntityType::Player;
    }

    const string& GetName() const;

    void Init() final;
    void Save(ostream& ofs) const final;
    void Load(istream& ifs) final;

private:
    string name = "";
};

extern Player player;

#endif /* Player_hpp */
