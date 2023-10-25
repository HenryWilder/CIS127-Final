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

string BaseNPC::GenName(int nameID) const
{
    constexpr cstring_t firstNames[] =
    {
        "James",
        "Jace",
        "Julian",
        "Jude",
        "Jasmine",
        "Jesse",
        "Jamie",
        "Jagger",
        "Jack",
        "Jade",
        "Jacob",
        "Joshua",
        "Jonah",
        "Jacqueline",
        "Jeremy",
        "John",
        "Jasper",
        "Jayden",
        "Jay",
        "Jaden",
        "Joel",
        "Jane",
        "Joseph",
        "June",
        "Judah",
        "Joseph",
        "Juliette",
        "Javier",
        "Journee",
        "Juniper",
        "Jason",
        "Jacalyn",
        "Janey",
        "Jango",
        "Janez",
        "Janiece",
        "Janiel",
        "Janifer",
        "Jelle",
        "Jemal",
        "Jemall",
        "Jemar",
        "Jem",
        "Jeminah",
        "Jemmie",
        "Jemma",
        "Jemarr",
        "Jillie",
        "Jim",
        "Jimbo",
        "Jillion",
        "Jorrell",
        "Jorry",
        "Jerry",
        "Jose",
        "Jos\x82",
        "Josep",
        "Joslynne",
        "Jospeh",
        "Josphine",
        "Joss",
        "Jossalyn",
        "Journee",
        "Journey",
        "Just", // pff "Just Baker"
        "Jovana",
        "Joy",
        "Joyann",
        "Jowell",
        "Judi",
        "Judy",
        "Juel",
        "Judyth",
        "Julet",
        "Julia",
        "Justin",
        "Justine",
        "Julie",
        "Julio",
        "J\xA3lio",
        "July",
        "Junio",
        "Juno",
        "Jupiter",
        "Jaclyn",
    };

    size_t firstNameIndex = nameID % _countof(firstNames);
    cstring_t firstNamePick = firstNames[firstNameIndex];

    return format("{} {}", firstNameIndex, GetLastName());
}


void BakerNPC::DoInteraction(Player& player)
{
    cout << "Hi, I'm " << GetDisplayName() << '\n';
}

void BlacksmithNPC::DoInteraction(Player& player)
{
    cout << "Hi, I'm " << GetDisplayName() << '\n';
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
