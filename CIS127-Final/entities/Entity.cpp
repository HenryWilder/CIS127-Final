#include "utilities/pch/utilities.hpp"
#include "Entity.hpp"
#include "Surroundings.hpp"
#include "echo.hpp"

#include "Door.hpp"
#include "Baker.hpp"
#include "Blacksmith.hpp"
#include "Wizard.hpp"
#include "Monster.hpp"
#include "Player.hpp"

void Entity::RemoveFromWorld( )
{
    surroundings.QueueForRemoval(GetType( ));
    echo << "[" << entityTypes.ValueAt(GetType( )) << " is now gone.]\n";
}

Entity *NewEntityOfType(EntityType type)
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
