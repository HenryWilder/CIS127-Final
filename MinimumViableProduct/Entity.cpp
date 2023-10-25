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

istream& operator>>(istream& stream, Entity*& entity)
{
    string typeName;
    stream >> typeName;
    if (typeName != "-")
    {
        entity = NewEntityOfType(Entity::TypeFromName(typeName));
        entity->FromStream(stream);
    }
    else
    {
        entity = nullptr;
    }
    return stream;
}

ostream& operator<<(ostream& stream, const Entity* entity)
{
    if (entity)
    {
        stream << entity->GetTypeName() << ' ';
        entity->ToStream(stream);
    }
    else
    {
        stream << "-";
    }
    return stream;
}
