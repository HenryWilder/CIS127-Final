#ifndef Player_hpp
#define Player_hpp

#include "utilities.hpp"
#include "Components.hpp"
#include "Interactable.hpp"

class Player :
    public Entity
{
protected:
    void DoInteraction_Talk_Generic        () override;
    void DoInteraction_Talk_WineFish       () override;
    void DoInteraction_Talk_SkeleStock     () override;
    void DoInteraction_Talk_WoodpeckerSiege() override;
    void DoInteraction_Talk_SmithTeleken   () override;
    void DoInteraction_Talk_PenguinBattle  () override;
    void DoInteraction_Talk_Woodchuck      () override;
    
    void DoInteraction_Potion_Predict() override;
    void DoInteraction_Potion_Heal   () override;
    void DoInteraction_Potion_Water  () override;
    void DoInteraction_Potion_Wish   () override;
    void DoInteraction_Potion_Ducks  () override;
    void DoInteraction_Potion_Force  () override;
    void DoInteraction_Potion_Salt   () override;
    void DoInteraction_Potion_Ants   () override;
    void DoInteraction_Potion_Demon  () override;
    void DoInteraction_Potion_Fire   () override;
    void DoInteraction_Potion_Explode() override;
    void DoInteraction_Potion_Tree   () override;
    
public:
    void DoInteraction_Grab () override;
    void DoInteraction_Bread() override;
    void DoInteraction_Sword() override;
    void DoInteraction_Gold () override;

    constexpr EntityType GetType() const override
    {
        return EntityType::Player;
    }

    const string& GetName() const;

    void Init();
    void Save(ostream& ofs) const;
    void Load(istream& ifs);
    
public: // Properties
    Health     health;
    Inventory  inventory;
    Influences influences;
    Luck       luck;
    
private:
    string name = "";
};

extern Player player;

#endif /* Player_hpp */
