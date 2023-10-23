#include "Map.h"

void Map::DoMovement(Player* player)
{
    PromptOptionList options;

    // Anonymous structure - easier to use thanks to C++17 structured binding
    struct { string name; int x, y; }
    const directions[] =
    {
        { "west",  -1,  0 },
        { "east",  +1,  0 },
        { "north",  0, -1 },
        { "south",  0, +1 },
    };

    // Add/remove movement options conditionally
    {
        for (size_t i = 0; i < _countof(directions); ++i)
        {
            const auto& [directionName, xOffset, yOffset] = directions[i];

            int xPrime = player->x + xOffset;
            int yPrime = player->y + yOffset;

            if (!IsValidTile(xPrime, yPrime))
            {
                continue;
            }

            const Entity* entity = GetEntityAtPosition(xPrime, yPrime);

            if (entity) // Implied "!= nullptr" because nullptr is 0
            {
                string categoryName = entity->GetCategoryName();
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
        string selectedName = it->input;
        size_t selectedIndex = it - options.begin();

        int xPrime, yPrime;
        for (const auto& [directionName, xOffset, yOffset] : directions)
        {
            if (selectedName.starts_with(directionName))
            {
                xPrime = player->x + xOffset;
                yPrime = player->y + yOffset;
            }
        }

        // Explicit movement - never has a space
        if (selectedName.find(' ') == string::npos)
        {
            player->x = xPrime;
            player->y = yPrime;
            return;
        }

        // Entity interaction
        {
            Entity* entity = entities[EntityIndexFromPosition(xPrime, yPrime)];
            _ASSERTE(!!entity);
            entity->DoInteraction(player);
            return;
        }
    }
}

bool Map::IsValidTile(int x, int y) const
{
    // todo
    return true;
}

const Entity* Map::GetEntityAtPosition(int x, int y) const
{
    for (Entity* entity : entities)
    {
        if (entity->IsAtPosition(x, y))
        {
            return entity;
        }
    }
    return nullptr;
}

Entity* Map::GetEntityAtPosition(int x, int y)
{
    for (Entity* entity : entities)
    {
        if (entity->IsAtPosition(x, y))
        {
            return entity;
        }
    }
    return nullptr;
}

size_t Map::EntityIndexFromPosition(int x, int y) const
{
    const size_t numEntities = entities.size();
    for (size_t i = 0; i < numEntities; ++i)
    {
        if (entities[i]->IsAtPosition(x, y))
        {
            return i;
        }
    }
    return numEntities;
}

ostream& operator<<(ostream& stream, const Entity& map)
{
    // todo
    return stream;
}

istream& operator>>(istream& stream, Entity& map)
{
    // todo
    return stream;
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
