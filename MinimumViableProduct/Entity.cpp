#include "Entity.h"

EntityType Entity::TypeFromName(string typeName)
{
    if (typeName == "undefined"); // Jumps straight to end

    else if (typeName ==      BakerNPC().GetTypeName()) return EntityType::NPC_BAKER;
    else if (typeName == BlacksmithNPC().GetTypeName()) return EntityType::NPC_BLACKSMITH;

    return EntityType::_ENTITY; // Null entity
}


void BaseNPC::DoTrade(Player& player)
{
    // todo
}


void BakerNPC::DoInteraction(Player& player)
{
    // todo
}

void BlacksmithNPC::DoInteraction(Player& player)
{
    // todo
}


// Allocates and returns a new entity of the named type.
// Used for loading from a file.
_Ret_maybenull_ inline Entity* NewEntityOfType(EntityType type)
{
    switch (type)
    {
    case EntityType::NPC_BAKER:      return new BakerNPC();
    case EntityType::NPC_BLACKSMITH: return new BlacksmithNPC();
    default: return nullptr;
    }
}

istream& operator>>(istream& stream, _Out_ Entity*& entity)
{
    string categoryName;
    stream >> categoryName;
    entity = NewEntityOfType(Entity::TypeFromName(categoryName));
    assert(!!entity, "File corrupted");
    entity->FromStream(stream);
    return stream;
}

ostream& operator<<(ostream& stream, _In_ const Entity& entity)
{
    stream << entity.GetTypeName() << ' ';
    entity.ToStream(stream);
    return stream;
}
