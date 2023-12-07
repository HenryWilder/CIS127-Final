#include "utilities.hpp"
#include "Interactable.hpp"
#include "Surroundings.hpp"
#include "TurnEchoStream.hpp"

#include "Door.hpp"
#include "Baker.hpp"
#include "Blacksmith.hpp"
#include "Wizard.hpp"
#include "Monster.hpp"
#include "Player.hpp"

void Entity::DoInteraction_Talk_WineFish( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_SkeleStock( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_WoodpeckerSiege( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_NecroFarm( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_Bloodmoon( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_MoleMountain( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_MindflayerRecipe( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_GodFistfight( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_Theseus( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_SmithTeleken( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_NoGarlic( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_PenguinBattle( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_PetMentalHealth( )
{
    DoInteraction_Talk_Generic( );
}
void Entity::DoInteraction_Talk_Woodchuck( )
{
    DoInteraction_Talk_Generic( );
}

void Entity::DoInteraction_Talk(Topic topic)
{
    switch (topic)
    {
        case Topic::WineFish:          DoInteraction_Talk_WineFish        ( ); break;
        case Topic::SkeleStock:        DoInteraction_Talk_SkeleStock      ( ); break;
        case Topic::WoodpeckerSiege:   DoInteraction_Talk_WoodpeckerSiege ( ); break;
        case Topic::NecroFarm:         DoInteraction_Talk_NecroFarm       ( ); break;
        case Topic::Bloodmoon:         DoInteraction_Talk_Bloodmoon       ( ); break;
        case Topic::MoleMountain:      DoInteraction_Talk_MoleMountain    ( ); break;
        case Topic::MindflayerRecipe:  DoInteraction_Talk_MindflayerRecipe( ); break;
        case Topic::GodFistfight:      DoInteraction_Talk_GodFistfight    ( ); break;
        case Topic::Theseus:           DoInteraction_Talk_Theseus         ( ); break;
        case Topic::BlacksmithTeleken: DoInteraction_Talk_SmithTeleken    ( ); break;
        case Topic::NoGarlic:          DoInteraction_Talk_NoGarlic        ( ); break;
        case Topic::PenguinBattle:     DoInteraction_Talk_PenguinBattle   ( ); break;
        case Topic::PetMentalHealth:   DoInteraction_Talk_PetMentalHealth ( ); break;
        case Topic::Woodchuck:         DoInteraction_Talk_Woodchuck       ( ); break;
        default:                       DoInteraction_Talk_Generic         ( ); break;
    }
}

void Entity::RemoveFromWorld( )
{
    surroundings.QueueForRemoval(GetType( ));
    echo << "[" << entityTypes.ValueAt(GetType( )) << " is now gone.]\n";
}

Entity *NewInteractableOfType(EntityType type)
{
    // Todo: Item entities
    switch (type)
    {
        case EntityType::Bread:      return nullptr;
        case EntityType::Potion:     return nullptr;
        case EntityType::Gold:       return nullptr;
        case EntityType::Player:     return nullptr;

        case EntityType::Door:       return new Door      ( );
        case EntityType::Baker:      return new Baker     ( );
        case EntityType::Blacksmith: return new Blacksmith( );
        case EntityType::Wizard:     return new Wizard    ( );
        case EntityType::Monster:    return new Monster   ( );
    }
    throw new NotImplementedException(entityTypes.KeyAt(type));
}

NPC *NewNPCOfType(NPCType type, Collective collective)
{
    switch (type)
    {
        case NPCType::Baker:      return new Baker     (collective);
        case NPCType::Blacksmith: return new Blacksmith(collective);
        case NPCType::Wizard:     return new Wizard    (collective);
        case NPCType::Monster:    return new Monster   ( ); // Always "monsters"
    }
    throw new NotImplementedException(npcTypes.KeyAt(type));
}

void ItemEntity::Collect( )
{
    player.AddItem(GetItemEnum( ));
    RemoveFromWorld( );
}
