#ifndef Baker_hpp
#define Baker_hpp

#include "Interactable.hpp"

class Baker :
    public Interactable
{
protected:
    void DoInteraction_Grab () override;
    void DoInteraction_Bread() override;
    void DoInteraction_Sword() override;
    void DoInteraction_Gold () override;
    
    void DoInteraction_Talk_Generic         () override;
    void DoInteraction_Talk_WineFish        () override;
    void DoInteraction_Talk_SkeleStock      () override;
    void DoInteraction_Talk_WoodpeckerSiege () override;
    void DoInteraction_Talk_NecroFarm       () override;
    void DoInteraction_Talk_Bloodmoon       () override;
    void DoInteraction_Talk_MoleMountain    () override;
    void DoInteraction_Talk_MindflayerRecipe() override;
    void DoInteraction_Talk_GodFistfight    () override;
    void DoInteraction_Talk_Theseus         () override;
    void DoInteraction_Talk_SmithTeleken    () override;
    void DoInteraction_Talk_NoGarlic        () override;
    void DoInteraction_Talk_PenguinBattle   () override;
    void DoInteraction_Talk_PetMentalHealth () override;
    void DoInteraction_Talk_Woodchuck       () override;
    
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
    constexpr Baker() {}
    
    string GetName() const override;
};

#endif /* Baker_hpp */
