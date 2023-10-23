#include "Map.h"
#include "Entity.h"
#include "Character.h"

void Map::DoMovement(Player* player)
{
    PromptOptionList options;

    // Anonymous structure - easier to use thanks to C++17 structured binding
    struct { string name; IVec2 offset; }
    const directions[] =
    {
        { "west",  { -1,  0 } },
        { "east",  { +1,  0 } },
        { "north", {  0, -1 } },
        { "south", {  0, +1 } },
    };

    // Add/remove movement options conditionally
    {
        for (size_t i = 0; i < _countof(directions); ++i)
        {
            const auto& [directionName, offset] = directions[i];

            IVec2 posPrime = player->position + offset;

            Tile tile = GetTile(posPrime);

            if (tile.isWall)
            {
                continue;
            }

            if (tile.entity) // Implied "!= nullptr" because nullptr is 0
            {
                string categoryName = tile.entity->GetCategoryName();
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

    // Handle input
    {
        auto it = Prompt("Where", options);
        string selectedName  = it->input;
        size_t selectedIndex = it - options.begin();

        IVec2 posPrime;
        for (const auto& [directionName, offset] : directions)
        {
            if (selectedName.starts_with(directionName))
            {
                posPrime = player->position + offset;
            }
        }

        // Explicit movement - never has a space
        if (selectedName.find(' ') == string::npos)
        {
            player->position = posPrime;
            return;
        }

        Entity* entity = GetTile(posPrime).entity;
        _ASSERTE(!!entity); // There must be an entity if there is a space in selectedName

        // Entity interaction
        entity->DoInteraction(player);
    }
}

Tile Map::GetTile(IVec2 position)
{
    auto it = tiles.find(position);
    if (it == tiles.end()) // Generate new tile
    {
        // The space will generate the same no matter what route you take to reach it
        srand(seed ^ position.x ^ position.y);
        int r = rand();

        constexpr int WALL_BIT = 128;
        constexpr int NUM_ENTITY_TYPES = 1;
        static_assert(NUM_ENTITY_TYPES <= WALL_BIT,
            "Entities greater than WALL_BIT will necessarily only spawn in walls (and therefore won't spawn at all)");

        bool isWall = r & WALL_BIT;
        Entity* tileEntity;

        if (isWall)
        {
            tileEntity = nullptr;
        }
        else
        {
            switch (r % NUM_ENTITY_TYPES)
            {
            default: tileEntity = nullptr; break;
                // todo: cases for each entity type
            }
        }

        tiles.insert({ position, { isWall, tileEntity } });
    }
    return it->second;
}

void Map::Free()
{
    for (auto& [_, tile] : tiles)
    {
        if (tile.entity)
        {
            delete tile.entity;
        }
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
