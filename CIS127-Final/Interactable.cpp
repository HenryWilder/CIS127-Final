#include "Interactable.hpp"
#include "utilities.hpp"

void Interactable::DoInteraction_Talk_WineFish        () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_SkeleStock      () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_WoodpeckerSiege () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_NecroFarm       () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_Bloodmoon       () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_MoleMountain    () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_MindflayerRecipe() { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_GodFistfight    () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_Theseus         () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_SmithTeleken    () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_NoGarlic        () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_PenguinBattle   () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_PetMentalHealth () { DoInteraction_Talk_Generic(); }
void Interactable::DoInteraction_Talk_Woodchuck       () { DoInteraction_Talk_Generic(); }

void Interactable::DoInteraction_Talk(const string& topic)
{
    if (topic == "winefish"         ) DoInteraction_Talk_WineFish        ();
    if (topic == "skelestock"       ) DoInteraction_Talk_SkeleStock      ();
    if (topic == "woodpecker_siege" ) DoInteraction_Talk_WoodpeckerSiege ();
    if (topic == "necrofarm"        ) DoInteraction_Talk_NecroFarm       ();
    if (topic == "bloodmoon"        ) DoInteraction_Talk_Bloodmoon       ();
    if (topic == "mole_mountain"    ) DoInteraction_Talk_MoleMountain    ();
    if (topic == "mindflayer_recipe") DoInteraction_Talk_MindflayerRecipe();
    if (topic == "god_fistfight"    ) DoInteraction_Talk_GodFistfight    ();
    if (topic == "theseus"          ) DoInteraction_Talk_Theseus         ();
    if (topic == "smith_teleken"    ) DoInteraction_Talk_SmithTeleken    ();
    if (topic == "no_garlic"        ) DoInteraction_Talk_NoGarlic        ();
    if (topic == "penguin_battle"   ) DoInteraction_Talk_PenguinBattle   ();
    if (topic == "pet_mental_health") DoInteraction_Talk_PetMentalHealth ();
    if (topic == "woodchuck"        ) DoInteraction_Talk_Woodchuck       ();
    else DoInteraction_Talk_Generic();
}

void Interactable::DoInteraction_Potion(const string& effect)
{
    if      (effect == "predict") DoInteraction_Potion_Predict();
    else if (effect == "heal"   ) DoInteraction_Potion_Heal   ();
    else if (effect == "water"  ) DoInteraction_Potion_Water  ();
    else if (effect == "wish"   ) DoInteraction_Potion_Wish   ();
    else if (effect == "ducks"  ) DoInteraction_Potion_Ducks  ();
    else if (effect == "force"  ) DoInteraction_Potion_Force  ();
    else if (effect == "salt"   ) DoInteraction_Potion_Salt   ();
    else if (effect == "ants"   ) DoInteraction_Potion_Ants   ();
    else if (effect == "demon"  ) DoInteraction_Potion_Demon  ();
    else if (effect == "fire"   ) DoInteraction_Potion_Fire   ();
    else if (effect == "explode") DoInteraction_Potion_Explode();
    else if (effect == "tree"   ) DoInteraction_Potion_Tree   ();
    else throw new NotImplementedException(effect);
}

void Interactable::DoInteraction(const string& action, const string& topicOrEffect)
{
    if      (action == "talk"  ) DoInteraction_Talk  (topicOrEffect);
    else if (action == "grab"  ) DoInteraction_Grab  ();
    else if (action == "bread" ) DoInteraction_Bread ();
    else if (action == "sword" ) DoInteraction_Sword ();
    else if (action == "gold"  ) DoInteraction_Gold  ();
    else if (action == "potion") DoInteraction_Potion(topicOrEffect);
    else throw new NotImplementedException(action);
}

#include "Door.hpp"
#include "Baker.hpp"
#include "Blacksmith.hpp"
#include "Wizard.hpp"
#include "Monster.hpp"

Interactable* NewInteractableOfType(const string& shortName)
{
    if      (shortName == "door"   ) return new Door      ();
    else if (shortName == "baker"  ) return new Baker     ();
    else if (shortName == "smith"  ) return new Blacksmith();
    else if (shortName == "wizard" ) return new Wizard    ();
    else if (shortName == "monster") return new Monster   ();
    else throw new NotImplementedException(shortName);
}

Interactable* NewNPCOfType(const string& shortName, const Collective& collective)
{
    if      (shortName == "baker"  ) return new Baker     (collective);
    else if (shortName == "smith"  ) return new Blacksmith(collective);
    else if (shortName == "wizard" ) return new Wizard    (collective);
    else if (shortName == "monster") return new Monster   (); // Always "monsters"
    else throw new NotImplementedException(shortName);
}
