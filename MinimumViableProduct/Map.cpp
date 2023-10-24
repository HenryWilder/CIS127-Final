#include "Map.h"
#include "Entity.h"
#include "Character.h"

void Map::_GetMovementOptionsFromPosition(PromptOptionList& options, IVec2 position)
{
    for (size_t i = 0; i < _countof(directions); ++i)
    {
        const auto& [directionName, offset] = directions[i];

        IVec2 posPrime = position + offset;

        Tile tile = GetTile(posPrime);

        if (tile.isWall)
        {
            continue;
        }

        if (tile.entity) // Implied "!= nullptr" because nullptr is 0
        {
            string categoryName = tile.entity->GetTypeName();
            PromptOption opt =
            {
                directionName + ' ' + categoryName,
                "Interact with the " + categoryName + " " + directionName + " of you"
            };
            options.push_back(opt);
        }
        else
        {
            PromptOption opt =
            {
                directionName,
                string("Move one space to the ") + directionName
            };
            options.push_back(opt);
        }
    }
}

void Map::DoMovement(Player& player)
{
    PromptOptionList options;
    _GetMovementOptionsFromPosition(options, player.GetPosition());

    // Handle input
    {
        auto it = Prompt("Where", options);
        string selectedName  = it->input;
        size_t selectedIndex = it - options.begin();

        IVec2 offset;
        for (const auto& [directionName, directionOffset] : directions)
        {
            if (selectedName.starts_with(directionName))
            {
                offset = directionOffset;
            }
        }

        // Explicit movement - never has a space
        if (selectedName.find(' ') == string::npos)
        {
            player.Move(offset);
            return;
        }

        Entity* entity = GetTile(player.GetPosition() + offset).entity;
        assert(!!entity, "There must be an entity if there is a space in " + selectedName);

        // Entity interaction
        entity->DoInteraction(player);
    }
}

Tile Map::GetTile(IVec2 position)
{
    if (auto it = tiles.find(position); it != tiles.end())
    {
        return it->second;
    }
    else // Generate new tile
    {
        constexpr int WALL_BIT = 0b10000000;
        constexpr int NUM_ENTITY_TYPES = (int)EntityType::_NUM;
        static_assert(NUM_ENTITY_TYPES <= WALL_BIT,
            "Entities greater than WALL_BIT will necessarily only spawn in walls and therefore won't spawn at all.");

        // The space will generate the same no matter what route you take to reach it
        srand(seed ^ (unsigned int)position.x ^ (unsigned int)position.y);
        int r = rand() & 0xFF;

        bool isWall = r & WALL_BIT;
        Entity* tileEntity = isWall ? nullptr : NewEntityOfType((EntityType)(r % NUM_ENTITY_TYPES));
        
        return tiles.emplace(position, Tile(isWall, tileEntity)).first->second;
    }
}

ostream& operator<<(ostream& stream, const Map& map)
{
    // todo
    return stream;
}

istream& operator>>(istream& stream, Map& map)
{
    // todo
    return stream;
}
